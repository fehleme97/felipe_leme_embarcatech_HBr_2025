# Teste unitário ADC - Sensor de temperatura Interno

## 📝 Teste ADC - Temperatura Interna 
Este projeto faz leituras e teste unitário do canal ADC, afim de verificar se a conversão de tensão para temperatura (graus Celsius) está adequada e dentro de um erro tolerável. 

O programa é feito em duas repartições, sendo elas podendo ser habilitada ou desabilitada.

 1. Teste unitário -> No CMakeLists, há a opção de testar individualmente o canal ADC responsável pela conversão de tensão em temperatura, sendo necessário a intervenção para qual fim o main () do código será utilizado. Neste caso, é necessário deixar habilitado o "TEST: Executável de Testes".
 2. Teste do sensor interno embutido -> No CMakeLists há também a possibilidade de flegar somente a leitura de temperatura, sem realizar o teste unitário, para isso, é necessário a intervenção para qual fim o main () do código será utilizado. Neste caso, é necessário deixar habilitado o "APP: Aplicação principal".

## 🎯 Objetivo
Este projeto demonstrar a temperatura em tempo real do processador RP2040, através de um sensor térmico embutido na própria placa da Raspberry Pi Pico W. Assim como também possibilita o usuário de realizar teste unitário do canal de conversão ADC de tensão em temperatura para validação. 

## Estrutura do Projeto 📂
```
/teste_unitario_adc
├──module_a                     # Funções reutilizáveis
|   │──adc_temp.c
|   │──adc_temp.h
├──test                         # Testes unitários com Unity.
|    │──test_adc_temp.c           
├──teste_unitario_adc           # Leitura do sensor de temperatura embutido em tempo real.
|    │──teste_unitario_adc.c    
├──Unity                        # Biblioteca responsável pelo teste unitário.
├──CMakeLists                   # CMakeLists do projeto.
├──README.md                    # Descrições gerais dos projetos
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
