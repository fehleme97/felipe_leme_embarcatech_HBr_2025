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
│──LICENSE              # Licença do repositório (MIT)
│──README.md            # Descrições gerais dos projetos
```

## 🔗 Projetos
Abaixo está a lista de projetos desenvolvidos. Cada um possui um diretório específico dentro do repositório:

- [Projeto 1 - Audiometria - Primeira Fase](./projetos/audiometria_primeira_fase/)
- [Projeto 2 - Contador Decrescente](./projetos/contador_decrescente/)
- [Projeto 3 - Leitura do Joystick](./projetos/Bitdoglab_joystick/)
- [Projeto 4 - Leitura do Sensor de Temperatura](./projetos/leitura_temperatura/)
- [Projeto 5 - Pisca Led - Arquitetura Modular](./projetos/led_pisca/)


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
