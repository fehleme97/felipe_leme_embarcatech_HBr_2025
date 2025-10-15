# 📦 PROJETO: HOMECARE — RASPBERRY PI 4 (MQTT + LOGGER CSV)

**Plataforma:** Raspberry Pi 4 (Raspberry Pi OS / Debian)  
**Componentes:** Broker Mosquitto, Logger Python (paho-mqtt), systemd

---

## 🧭 Sobre o projeto

Este repositório reúne apenas o que roda na **Raspberry Pi 4** do projeto Homecare.  
Funções principais:

- Executar o **Mosquitto** como **broker MQTT** local.
- Rodar um **logger em Python** que **assina tópicos** do dispositivo (BitDogLab / Pico W) e **salva CSVs** para análise/dashboard.
- Publicar um **serviço systemd** para iniciar tudo no boot e reiniciar em falhas.
- Disponibilizar **scripts de teste** para validar o pipeline rapidamente.

> O **firmware da BitDogLab (Raspberry Pi Pico W)** fica em repositório/pasta separado(a).

---

## 🗂️ Estrutura (arquivos essenciais)

```
homecare-rpi4/
├─ homecare.conf                 # Configuração do Mosquitto (broker)
├─ homecare_logger.py            # Logger MQTT -> CSV
├─ homecare-logger.service       # Serviço systemd
├─ requirements.txt              # Dependências Python do logger
└─ test_pub_sub.sh               # Script p/ publicar mensagens de teste
└─ README.md                     # Esse arquivo
```

> Você pode organizar em pastas como preferir. Os **destinos sugeridos** no sistema:
>
> - `/etc/mosquitto/conf.d/homecare.conf`
> - `/opt/homecare/homecare_logger.py`
> - `/etc/systemd/system/homecare-logger.service`
> - `/opt/homecare/data/` (CSVs)
> - `/opt/homecare/logs/` (logs do logger)

---

## 🧰 Requisitos

- **Mosquitto** e **mosquitto-clients**
- **Python 3** + `pip`
- Pacote Python: `paho-mqtt` (via `requirements.txt`)

Instalação rápida:
```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients python3-pip
python3 -m pip install -r requirements.txt
```

---

## 🌐 Tópicos MQTT e payloads

**Device-ID padrão:** `pico-homecare-01` (edite no `homecare_logger.py` se precisar)

- `homecare/bracelet/pico-homecare-01/vitals`  
  ```json
  {"ts_ms": 1694192553000, "bpm": 72.10, "spo2": 97.23}
  ```

- `homecare/bracelet/pico-homecare-01/event/emergency`  
  ```json
  {"ts_ms": 1694192520000, "src": "fall" }   // ou "button"
  ```

- `homecare/bracelet/pico-homecare-01/status`  
  ```json
  {"ts_ms": 1694192530000, "wifi": 1, "mqtt": 1, "uptime": 123456}
  ```

---

## 🗃️ CSVs gerados (schemas)

Criados em `/opt/homecare/data/`:

- **vitals.csv**  
  `ts_iso,ts_ms,device_id,bpm,spo2`  
  `2025-09-08T19:42:33Z,355363,pico-homecare-01,72.10,97.23`

- **events.csv**  
  `ts_iso,ts_ms,device_id,src`  
  `2025-09-08T19:42:00Z,354298,pico-homecare-01,button`

- **status.csv**  
  `ts_iso,ts_ms,device_id,wifi,mqtt,uptime`  
  `2025-09-08T19:42:30Z,355000,pico-homecare-01,1,1,123456`

---

## ⚙️ Instalação e configuração

1) **Broker Mosquitto**
```bash
sudo mkdir -p /etc/mosquitto/conf.d
sudo cp homecare.conf /etc/mosquitto/conf.d/
sudo systemctl enable mosquitto
sudo systemctl restart mosquitto
```

2) **Diretórios do logger**
```bash
sudo mkdir -p /opt/homecare/data /opt/homecare/logs
sudo cp homecare_logger.py /opt/homecare/
```

3) **Dependências Python**
```bash
python3 -m pip install -r requirements.txt
```

4) **Service systemd**
```bash
sudo cp homecare-logger.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable homecare-logger.service
sudo systemctl restart homecare-logger.service
sudo systemctl status homecare-logger.service --no-pager
```

---

## 🧪 Teste rápido (publicando mensagens)

Em um terminal:
```bash
mosquitto_sub -t 'homecare/#' -v
```

Em outro terminal, rode o script de exemplo:
```bash
chmod +x test_pub_sub.sh
./test_pub_sub.sh localhost
```

Você deve ver as mensagens chegando e os CSVs sendo preenchidos em `/opt/homecare/data/`.

---

## 🔍 Logs e troubleshooting

- **Logs do broker**  
  ```bash
  sudo journalctl -u mosquitto -f
  ```

- **Logs do logger**  
  ```bash
  tail -f /opt/homecare/logs/homecare-logger.log
  ```

- **CSV não atualiza**  
  - Confirme o service ativo: `systemctl status homecare-logger.service`  
  - Permissões de `/opt/homecare/data` e `/opt/homecare/logs`  
  - Tópicos e `DEVICE_ID` iguais aos do firmware  
  - Teste publish: `test_pub_sub.sh` (ou `mosquitto_pub`)

- **MQTT offline / rede caiu**  
  - O Mosquitto deve reiniciar: `sudo systemctl restart mosquitto`  
  - Logger reconecta automaticamente (inicie/pare o service se necessário)

---

## 🔐 Segurança (produção)

O `homecare.conf` está **aberto** (laboratório): `allow_anonymous true`.  
Para produção, recomenda-se:

- Desabilitar anônimo e usar usuário/senha:
  ```conf
  allow_anonymous false
  password_file /etc/mosquitto/passwd
  ```
  ```bash
  sudo mosquitto_passwd -c /etc/mosquitto/passwd usuario
  sudo systemctl restart mosquitto
  ```

- Habilitar **TLS** (porta 8883) e **ACLs** se houver múltiplos clientes e permissões distintas.

---

## 🔧 Customizações úteis

- **Trocar DEVICE_ID** (logger)  
  Edite no `homecare_logger.py`:
  ```python
  DEVICE_ID = 'pico-homecare-01'
  ```
  E ajuste os tópicos se necessário.

- **Mudar caminhos de dados/logs**  
  Altere `DATA_DIR` e `LOG_DIR` no `homecare_logger.py`.

- **Vários dispositivos**  
  Você pode rodar **múltiplos loggers** (um por device) com services duplicados:  
  copie o `homecare_logger.py` e o `.service`, ajuste `DEVICE_ID` e `ExecStart`.

---

## 🧽 Desinstalação

```bash
sudo systemctl disable --now homecare-logger.service
sudo rm -f /etc/systemd/system/homecare-logger.service
sudo systemctl daemon-reload

# (opcional)
sudo rm -rf /opt/homecare
```

---

## 🧱 Tecnologias utilizadas

- **Mosquitto** (broker MQTT)
- **Python 3** + **paho-mqtt**
- **systemd** (serviço e reinício automático)
- Formato **CSV** para integração com dashboards

---

## 📜 Licença

**MIT** 
