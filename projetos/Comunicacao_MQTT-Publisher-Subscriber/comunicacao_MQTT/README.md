# 📄 PROJETO: PUBLICADOR MQTT COM SEGURANÇA BÁSICA - BITDOGLAB (PICO W)

Autores: 
- FELIPE LEME CORREA DA SILVA
- VITOR GOMES SOUZA 

Instituição: EmbarcaTech - HBr - Campinas

# 📦 SOBRE O PROJETO

Este projeto implementa um Publicador MQTT no microcontrolador BitDogLab (Raspberry Pi Pico W). O sistema se conecta a uma rede Wi-Fi, configura um endereço IP fixo, e realiza publicações periódicas de dados sensoriais (temperatura), com funcionalidades adicionais de segurança e proteção, incluindo:

Ofuscação de dados com criptografia XOR.

Prevenção de ataques de replay através da inserção de timestamp nas mensagens.

Autenticação via MQTT com usuário e senha.

Publicação de dados via protocolo MQTT usando a pilha lwIP.

# 🗂️ Estrutura do Projeto

```
/comunicacao_MQTT
│── inc                  # Bibliotecas específicas (mqtt_comm, lwipopts, etc.)
│── src                  # Biblioteca específica .c
│── comunicacao_MQTT.c   # Código fonte principal
│── CMakeLists.txt       # Configuração de compilação
│── README.md            # Este documento
│── LICENSE.txt          # Licença do projeto
``` 

# 🛠️ Funcionalidades Implementadas

✅ Conexão à rede Wi-Fi com IP fixo
✅ Publicação periódica no tópico MQTT escola/sala1/temperatura
✅ Mensagens enviadas são criptografadas com XOR
✅ Cada mensagem contém um timestamp, evitando replay
✅ Comunicação segura autenticada no broker Mosquitto

# 🌐 Configurações Utilizadas

- SSID Wi-Fi
- Senha Wi-Fi
- Broker MQTT
- Chave XOR: 42

# 🔗 Fluxo da Publicação

1. Conexão ao Wi-Fi.
2. Configuração de IP fixo (192.168.15.100).
3. Geração de mensagem no formato:
    - {"valor": 26.5, "ts": <timestamp>}
4. Criptografia da mensagem com XOR.
5. Publicação via MQTT.

# 🖥️ Execução e Monitoramento

1. Iniciar o broker Mosquitto:
```
mosquitto -c C:\Users\Felipe\Desktop\comunicacao_MQTT\mosquitto\mosquitto.conf -v
```

2. Conectar o Pico W e monitorar via Serial:

    - Exibe status de conexão e confirmações de envio.

3. Observar a publicação com:
```
mosquitto_sub -h 192.168.15.54 -u aluno -P senha123 -t "escola/sala1/temperatura" -v
```

# 🛠️ Compilação

```
mkdir build
cd build
cmake ..
make
```

Depois, grave o .uf2 no Pico W.

# 📌 Tecnologias Utilizadas

- C/C++

- Raspberry Pi Pico SDK

- lwIP (para comunicação TCP/IP)

- Mosquitto MQTT Broker

# 📜 Licença

- GNU (GENERAL PUBLIC LICENSE) 
