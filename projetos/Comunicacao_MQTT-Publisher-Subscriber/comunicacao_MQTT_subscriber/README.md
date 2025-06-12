# 📄 PROJETO: SUBSCRIBER MQTT COM DECIFRAÇÃO E PROTEÇÃO REPLAY - BITDOGLAB (PICO W)

Autores:
- FELIPE LEME CORREA DA SILVA
- VITOR GOMES SOUZA

Instituição: EmbarcaTech - HBr - Campinas

# 📦 SOBRE O PROJETO

Este projeto implementa um Subscriber MQTT no BitDogLab (Raspberry Pi Pico W), que realiza:

- Assinatura do tópico escola/sala1/temperatura.
- Recepção e decifração de mensagens com criptografia XOR.
- Proteção contra ataques de replay validando timestamp.
- Exibe as leituras válidas e descarta mensagens repetidas.

# 🗂️ Estrutura do Projeto

```
/subscriber_MQTT
│── include              # Bibliotecas específicas (mqtt_comm, lwipopts, etc.)
│── src                  # Biblioteca específica .c
│── subscriber_MQTT.c    # Código fonte principal
│── CMakeLists.txt       # Configuração de compilação
│── README.md            # Este documento
│── LICENSE.txt          # Licença do projeto
``` 

# 🛠️ Funcionalidades Implementadas

✅ Conexão à rede Wi-Fi com IP fixo
✅ Subscrição ao tópico MQTT
✅ Decifração da mensagem usando XOR
✅ Proteção contra replay baseada em timestamp
✅ Exibição das mensagens válidas no Serial Monitor

# 🌐 Configurações Utilizadas

- SSID Wi-Fi
- Senha Wi-Fi
- Broker MQTT
- Chave XOR: 42

# 🔗 Fluxo da Publicação

1. Conexão à rede Wi-Fi.
2. Subscrição ao tópico escola/sala1/temperatura.
3. Recepção de mensagem criptografada.
4. Decifração via XOR.
5. Verificação do timestamp:
    - Se novo, processa.
    - Se igual ou anterior, descarta como replay.

# 🖥️ Execução e Monitoramento

1. Iniciar o broker Mosquitto:
```
mosquitto -c C:\Users\Felipe\Desktop\comunicacao_MQTT\mosquitto\mosquitto.conf -v 
```

2. Conectar o Pico W e monitorar via Serial:

    - Exibe status de conexão e confirmações de envio.

3. Observar a publicação com:
```
mosquitto_pub -h 192.168.15.54 -u aluno -P senha123 -t "escola/sala1/temperatura" -m "teste"
```

🛠️ Compilação
```
mkdir build
cd build
cmake ..
make
```

Depois, grave o .uf2 no Pico W.

# 📌 Tecnologias Utilizadas

C/C++

Raspberry Pi Pico SDK

lwIP (para comunicação TCP/IP)

Mosquitto MQTT Broker

# 📜 Licença

- GNU (GENERAL PUBLIC LICENSE) 
