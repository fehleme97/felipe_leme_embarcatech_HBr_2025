#!/usr/bin/env python3
import os
import csv
import time
import json
import threading
from datetime import datetime
import paho.mqtt.client as mqtt

# Configurações do broker MQTT
BROKER = "ip"         # IP do broker Mosquitto
PORT = 1883
USERNAME = "user"     # usuário
PASSWORD = "psw"      # senha

# Tópicos
DEVICE_ID = "pico-homecare-01"
TOPIC_VITALS    = f"homecare/bracelet/{DEVICE_ID}/vitals"
TOPIC_FALL      = f"homecare/bracelet/{DEVICE_ID}/event/fall"
TOPIC_EMERGENCY = f"homecare/bracelet/{DEVICE_ID}/event/emergency"

# Arquivo CSV
OUT_FILE = "dados.csv"

# Tempo em segundos que "queda" ou "emergencia" devem permanecer =1 antes de voltar a 0
QUEDA_TIMEOUT = 30
EMERGENCIA_TIMEOUT = 20

# Estado atual
batimentos = None
oxigenacao = None
queda = 0
quedas = 0
emergencia = 0

# Inicialização do CSV
if not os.path.exists(OUT_FILE):
    with open(OUT_FILE, "w", newline="", encoding="utf-8") as g:
        w = csv.writer(g)
        w.writerow(["batimentos", "oxigenação", "horário", "queda", "quedas", "emergencia"])
    print(f"📄 Criado {OUT_FILE} com cabeçalho.")

# Funções auxiliares
def save_csv():
    """Grava uma linha no CSV com os dados atuais"""
    global batimentos, oxigenacao, queda, quedas, emergencia

    if batimentos is None or oxigenacao is None:
        return  # ainda não recebemos vitals

    horario = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    with open(OUT_FILE, "a", newline="", encoding="utf-8") as g:
        w = csv.writer(g)
        w.writerow([batimentos, oxigenacao, horario, queda, quedas, emergencia])

    print(f"💾 CSV atualizado -> batimentos={batimentos}, "
          f"oxigenação={oxigenacao}, queda={queda}, quedas={quedas}, emergencia={emergencia}, horário={horario}")

def reset_queda():
    """Reseta 'queda' para 0 após QUEDA_TIMEOUT segundos"""
    global queda
    queda = 0
    save_csv()
    print(f"🔄 Queda resetada para 0 após {QUEDA_TIMEOUT}s")

def reset_emergencia():
    """Reseta 'emergencia' para 0 após EMERGENCIA_TIMEOUT segundos"""
    global emergencia
    emergencia = 0
    save_csv()
    print(f"🔄 Emergência resetada para 0 após {EMERGENCIA_TIMEOUT}s")

# Callbacks MQTT
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print(f"✅ Conectado ao broker {BROKER}:{PORT}")
        client.subscribe(TOPIC_VITALS)
        client.subscribe(TOPIC_FALL)
        client.subscribe(TOPIC_EMERGENCY)
        print(f"📡 Assinado em: {TOPIC_VITALS}, {TOPIC_FALL}, {TOPIC_EMERGENCY}")
    else:
        print(f"❌ Erro de conexão. Código: {rc}")

def on_message(client, userdata, msg):
    global batimentos, oxigenacao, queda, quedas, emergencia

    payload_raw = msg.payload.decode().strip()
    print(f"\n📩 Mensagem recebida em {msg.topic}: {payload_raw}")

    try:
        payload = json.loads(payload_raw)
    except Exception:
        payload = {}

    # VITALS
    if msg.topic.endswith("/vitals"):
        try:
            batimentos = float(payload.get("bpm", 0.0))
            oxigenacao = float(payload.get("spo2", 0.0))
            print(f"   ✅ Vitals: batimentos={batimentos}, oxigenação={oxigenacao}")
            save_csv()
        except Exception as e:
            print("   ❌ Erro ao processar vitals:", e)

    # EVENTO: QUEDA
    elif msg.topic.endswith("/event/fall"):
        queda = 1
        quedas += 1
        print(f"   🚨 Queda detectada! Total quedas={quedas} (src={payload.get('src')})")
        save_csv()
        threading.Timer(QUEDA_TIMEOUT, reset_queda).start()

    #EVENTO: EMERGÊNCIA
    elif msg.topic.endswith("/event/emergency"):
        emergencia = 1
        print(f"   🚨 Emergência detectada! (src={payload.get('src')})")
        save_csv()
        threading.Timer(EMERGENCIA_TIMEOUT, reset_emergencia).start()

    else:
        print(f"   ⚠️ Tópico não reconhecido: {msg.topic}")

# Cliente MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.username_pw_set(USERNAME, PASSWORD)
client.on_connect = on_connect
client.on_message = on_message

print(f"🚀 Conectando ao broker MQTT {BROKER}:{PORT} como '{USERNAME}' ...")
client.connect(BROKER, PORT, 60)

# Loop infinito
client.loop_forever()