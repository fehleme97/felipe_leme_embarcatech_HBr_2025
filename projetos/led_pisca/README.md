## Pisca Led - Arquitetura Modular

# 📝 Blink - Arquitetura modular
Este projeto faz com que o LED embutido da Raspberry Pi Pico W pisque com período de 1 segundo. Sendo introdutório para programações estruturadas com arquitetura modular, conforme sugerido em atividade. 

## 🎯 Objetivo
Este projeto demonstra como organizar o código para o Raspberry Pi Pico W de forma modular, separando **drivers**, **HAL** (Hardware Abstraction Layer) e **aplicação**.

## Estrutura do Projeto 📂
```
/pisca_led
├──app
|   │──led_pisca.c
├──drivers
|    │──led_embutido.c        
├──hal
|    │──hal_led.c
├──include
|    │──led_embutido.h         
|    │──hal_led.h           
├──CMakeLists     # CMakeLists do projeto.
├──README.md           # Descrições gerais dos projetos
```

## 🖥️ Requisitos

- Raspberry Pi Pico W
- CMake >= 3.13
- Compilador GCC para ARM (ex: arm-none-eabi-gcc)
- `ninja` (opcional, mas recomendado para build rápido)
- SDK da Raspberry Pi Pico

## 🖥️ Como clonar o SDK

Antes de compilar, clone o pico-sdk:

bash
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

### Compilação

- mkdir build (Cria uma pasta build)
- cd build (Entra na pasta build)
- cmake .. -G Ninja (Roda o CMAke)
- ninja (Compila)
- O binário será gerado em build/projeto.uf2, pronto para ser gravado no Pico W!

---
## 📜 Licença
MIT License
