#!/usr/bin/env bash
set -e
BROKER="${1:-localhost}"
DEV="pico-homecare-01"
ts_ms=$(($(date +%s%N)/1000000))
echo "[Teste] Publicando mensagens de exemplo em $BROKER ..."
mosquitto_pub -h "$BROKER" -t "homecare/bracelet/$DEV/vitals" -m "{"ts_ms":$ts_ms,"bpm":72.1,"spo2":97.2}"
mosquitto_pub -h "$BROKER" -t "homecare/bracelet/$DEV/event/emergency" -m "{"ts_ms":$ts_ms,"src":"button"}"
mosquitto_pub -h "$BROKER" -t "homecare/bracelet/$DEV/status" -m "{"ts_ms":$ts_ms,"wifi":1,"mqtt":1,"uptime":123456}"
echo "OK. Use: mosquitto_sub -t 'homecare/#' -v"
