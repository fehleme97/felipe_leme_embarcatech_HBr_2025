# PROJETOS DESENVOLVIDOS NO CURSO EMBARCATECH 2025 

Autor: FELIPE LEME CORREA DA SILVA 

Instituição: EmbarcaTech - HBr - Campinas

## SOBRE O REPOSITÓRIO: 

AO DECORRER DESSE REPOSITÓRIO SERÁ APRESENTADO PROJETOS QUE FORAM DESENVOLVIDOS DURANTE O PROCESSO DE APRENDIZAGEM NO CURSO DE RESIDÊNCIA TECNOLÓGICA EM SISTEMAS EMBARCADOS.

## 📂 Lista de Projetos

```
/projetos
│──audiometria_primeira_fase
    │──assets          # Imagens, vídeos, esquemas
    │──docs            # Documentação técnica complementar
    │──inc             # Bibliotecas do projeto
    │── README.md      # Descrição específica do projeto
    │── BitDogLab_Audiometria.c     # Código fonte.
    │── CMakeLists     # CMakeLists do projeto.
│──contador_decrescente
    │──assets          # Imagens, vídeos, esquemas
    │──docs            # Documentação técnica complementar
    │──inc             # Bibliotecas do projeto
    │── README.md      # Descrição específica do projeto
    │── contador_decrescente.c     # Código fonte.
    │── CMakeLists     # CMakeLists do projeto. 
│──Bitdoglab_joystick
    │──assets          # Imagens, vídeos, esquemas
    │──docs            # Documentação técnica complementar
    │──inc             # Bibliotecas do projeto
    │── README.md      # Descrição específica do projeto
    │── Bitdoglab_joystick.c     # Código fonte.
    │── CMakeLists     # CMakeLists do projeto.
│──leitura_temperatura
    │──assets          # Imagens, vídeos, esquemas
    │──docs            # Documentação técnica complementar
    │──inc             # Bibliotecas do projeto
    │── README.md      # Descrição específica do projeto
    │── leitura_temperatura.c     # Código fonte.
    │── CMakeLists     # CMakeLists do projeto.
│──pisca_led
    │──app
    │──drivers       
    │──hal
    │──include      
    │──CMakeLists       # CMakeLists do projeto.
    │──README.md        # Descrições gerais dos projetos
│──teste_unitario_adc
    ├──module_a                     # Funções reutilizáveis
    ├──test                         # Testes unitários com Unity.        
    ├──teste_unitario_adc           # Leitura do sensor de temperatura embutido em tempo real.
    ├──Unity                        # Biblioteca responsável pelo teste unitário.
    ├──CMakeLists                   # CMakeLists do projeto.
    ├──README.md                    # Descrições gerais dos projetos
    │──README.md            # Descrições gerais dos projetos
│──galton_board
    ├──inc                     # Bibliotecas do projeto.
    ├──galton_board            # Código fonte.
    ├──CMakeLists              # CMakeLists do projeto.
    │──README.md               # Descrições gerais dos projetos
│──tarefa-freertos-1-fehleme97
    │── include/                             # Arquivos auxiliares
    │── FreeRTOS                             # Arquivos da biblioteca da FreeRTOS utilizado no projeto
    │── doc                                  # Arquivo contendo relatório com respotas sobre o sistema
    │── tarefa-freertos-1-fehleme97.c        # Código principal com FreeRTOS
    │── CMakeLists.txt                       # Script de build do projeto
    │── README.md                            # Este documento
    │── LICENSE.txt                          # Licença do projeto
│──sintetizador_de_audio
    │── inc/ # Bibliotecas personalizadas (ssd1306.h, ssd1306_font.h, etc.)
    │── sintetizador_de_audio.c # Código fonte principal
    │── CMakeLists.txt # Configuração de compilação
    │── README.md # Este documento
    │── LICENSE.txt # Licença do projeto
│──comunicacao_MQTT
    │── inc                  # Bibliotecas específicas (mqtt_comm, lwipopts, etc.)
    │── src                  # Biblioteca específica .c
    │── comunicacao_MQTT.c   # Código fonte principal
    │── CMakeLists.txt       # Configuração de compilação
    │── README.md            # Este documento
    │── LICENSE.txt          # Licença do projeto
│──subscriber_MQTT
    │── include              # Bibliotecas específicas (mqtt_comm, lwipopts, etc.)
    │── src                  # Biblioteca específica .c
    │── subscriber_MQTT.c    # Código fonte principal
    │── CMakeLists.txt       # Configuração de compilação
    │── README.md            # Este documento
    │── LICENSE.txt          # Licença do projeto
│──tarefa-freertos-2-felipe_e_vitor
    │── include              # Bibliotecas específicas 
    │── tarefa-freertos-2-felipe_e_vitor.c    # Código fonte principal
    │── CMakeLists.txt       # Configuração de compilação
    │── README.md            # Este documento
    │── LICENSE.txt          # Licença do projeto
│──motor_drive
    ├── motor_drive.c   # Código principal do controle de motores
    ├── CMakeLists.txt  # Script de build para o projeto
    ├── README.md       # Este arquivo
    ├── LICENSE.txt     # Licensa do projeto
│──projeto-final_felipe_vitor-main
    ├── code            # Códigos do projeto
    ├── documentation   # Documentações do projeto
    ├── README.md       # Este arquivo
    ├── LICENSE.txt     # Licensa do projeto
```

## 🔗 Projetos
Abaixo está a lista de projetos desenvolvidos. Cada um possui um diretório específico dentro do repositório:

- [Projeto 1 - Audiometria - Primeira Fase](./projetos/audiometria_primeira_fase/)
- [Projeto 2 - Contador Decrescente](./projetos/contador_decrescente/)
- [Projeto 3 - Leitura do Joystick](./projetos/Bitdoglab_joystick/)
- [Projeto 4 - Leitura do Sensor de Temperatura](./projetos/leitura_temperatura/)
- [Projeto 5 - Pisca Led - Arquitetura Modular](./projetos/led_pisca/)
- [Projeto 6 - Teste unitário - Sensor embutido de temperatura - Arquitetura Modular](./projetos/teste_unitario_adc)
- [Projeto 7 - Galton Board](./projetos/galton_board)
- [Projeto 8 - Tarefas FreeRTOS](./projetos/FreeRTOS_TAREFAS)
- [Projeto 9 - Sintetizador de Audio](./projetos/sintetizador_de_audio/)
- [Projeto 10 - Comunicação MQTT e criptografia](./projetos/Comunicacao_MQTT-Publisher-Subscriber/)
- [Projeto 11 - Tasks FreeRTOS](./projetos/tarefa-freertos-2-felipe_e_vitor/)
- [Projeto 12 - Controle - Motor Drive](./projetos/motor_drive)
- [Projeto 13 - Projeto final felipe e vitor - Monitoramento Homecare](./projeto-final-felipe_vitor-main)


## 🛠️ Requisitos e Configuração
Para compilar e rodar os projetos no **Raspberry Pi Pico W**, siga os passos abaixo:

1. **Instale o SDK do Pico** seguindo as instruções oficiais da Raspberry Pi.
2. Clone este repositório:
   ```bash
   git clone https://github.com/seu_usuario/seu_repositorio.git
   cd meu_projeto
   ```
3. **Crie o diretório de build e compile:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
4. **Carregue o firmware no Raspberry Pi Pico W.**
