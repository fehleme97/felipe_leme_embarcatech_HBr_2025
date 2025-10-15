#!/usr/bin/env python3
# Logger MQTT -> CSV (RPi4)
import json, os, time, csv
from datetime import datetime
import paho.mqtt.client as mqtt

DATA_DIR = '/opt/homecare/data'
LOG_DIR  = '/opt/homecare/logs'
BROKER   = 'localhost'
PORT     = 1883
DEVICE_ID = 'pico-homecare-01'

TOP_VITALS = f'homecare/bracelet/{DEVICE_ID}/vitals'
TOP_EVENT  = f'homecare/bracelet/{DEVICE_ID}/event/emergency'
TOP_STATUS = f'homecare/bracelet/{DEVICE_ID}/status'

os.makedirs(DATA_DIR, exist_ok=True)
os.makedirs(LOG_DIR, exist_ok=True)

def open_csv(path, header):
    exists = os.path.exists(path)
    f = open(path, 'a', newline='')
    w = csv.writer(f)
    if not exists:
        w.writerow(header)
    return f, w

fv, wv = open_csv(os.path.join(DATA_DIR,'vitals.csv'), ['ts_iso','ts_ms','device_id','bpm','spo2'])
fe, we = open_csv(os.path.join(DATA_DIR,'events.csv'), ['ts_iso','ts_ms','device_id','src'])
fs, ws = open_csv(os.path.join(DATA_DIR,'status.csv'), ['ts_iso','ts_ms','device_id','wifi','mqtt','uptime'])

def log_error(msg):
    with open(os.path.join(LOG_DIR,'homecare-logger.log'),'a') as lf:
        lf.write(f"{datetime.utcnow().isoformat()}Z ERROR {msg}\n")

def on_connect(client, userdata, flags, rc):
    client.subscribe([(TOP_VITALS,0), (TOP_EVENT,1), (TOP_STATUS,0)])

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode('utf-8'))
        ts_ms = int(payload.get('ts_ms', round(time.time()*1000)))
        ts_iso = datetime.utcfromtimestamp(ts_ms/1000.0).isoformat()+'Z'
        if msg.topic == TOP_VITALS:
            wv.writerow([ts_iso, ts_ms, DEVICE_ID, payload.get('bpm'), payload.get('spo2')]); fv.flush()
        elif msg.topic == TOP_EVENT:
            we.writerow([ts_iso, ts_ms, DEVICE_ID, payload.get('src')]); fe.flush()
        elif msg.topic == TOP_STATUS:
            ws.writerow([ts_iso, ts_ms, DEVICE_ID, payload.get('wifi'), payload.get('mqtt'), payload.get('uptime')]); fs.flush()
    except Exception as e:
        log_error(f"{e} | topic={msg.topic} payload={msg.payload[:200]}")

def main():
    c = mqtt.Client(client_id='homecare-logger')
    c.on_connect = on_connect
    c.on_message = on_message
    c.connect(BROKER, PORT, keepalive=60)
    c.loop_forever()

if __name__ == '__main__':
    main()
