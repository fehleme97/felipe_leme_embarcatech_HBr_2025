## PROJETO 4

# Sensor de Temperatura do RP2040 - Segunda Fase - Embarcatech
Este projeto implementa um sensor de temperatura para verificação da leitura em tempo real do microcontrolador RP2040. Além de converter para Celsius o valor lido.

## Objetivo
Fazer um leitor da temperatura do microcontrolador RP2040 para leitura em tempo real em Celsius, com:
- Display OLED;
- BitDogLab;
- Sensor de Temperatura. 


##  Lista de materiais: 

| Componente           | Conexão na BitDogLab |
|----------------------|----------------------|
| BitDogLab (RP2040)   | -                    |
| Display OLED I2C     | SDA: GPIO14 / SCL: GPIO15 |
| Sensor de Temperatura   | GPIO04                    |


Execução
Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM).


Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make).


Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo).


Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2).


A Pico reiniciará automaticamente e começará a executar o código.


As instruções ao usuário serão impressas no display OLED e posterior o contador irá começar.

## Lógica

- Inicialização:  

• O sistema inicia com uma mensagem no display OLED instruindo o usuário sobre
a leitura do sensor de temperatura convertido em Celsius. 

- Execução do Teste: 

• Ao iniciar a BitDogLab, o DISPLAY OLED imprimi uma instrução ao usuário informando
sobre o inicio da leitura da temperatura interna do microcontrolador RP2040. 

• A leitura é convertida em Celsius e impressa em tempo real.

##  Arquivos
- `projetos/leitura_temperatura/leitura_temperatura.c`: Código principal do projeto
- `projetos/leitura_temperatura/CMakeLists.txt`: Arquivo CMake que contém as diretrizes do projeto e as bibliotecas utilizadas. 
- `projetos/LICENSE.txt`: Licença de distribuição. 


---
## 📜 Licença
MIT License
