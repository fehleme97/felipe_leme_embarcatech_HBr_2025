# Leitura do Joystick

## 📝 Projeto de leitura em tempo real do Joystick - Segunda Fase - Embarcatech
Este projeto implementar a leitura do controle joystick através de um DISPLAY OLED para visualização em tempo real da movimentação feita.  

## 🎯 Objetivo
Fazer um leitor de movimento do joystick, com:
- Display OLED;
- Joystick; 

## 🛠️ Lista de materiais: 

| Componente           | Conexão na BitDogLab |
|----------------------|----------------------|
| BitDogLab (RP2040)   | -                    |
| Display OLED I2C     | SDA: GPIO14 / SCL: GPIO15 |
| JOYSTICK             | GPIO22                 |


## 🖥️ Execução
Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM).


Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make).


Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo).


Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2).


A Pico reiniciará automaticamente e começará a executar o código.


As instruções ao usuário serão impressas no display OLED.

## 🚀 Lógica

- Inicialização:  

• O sistema inicia com uma mensagem no display OLED instruindo o usuário a 
utilizar o joystick para imprimir o valor real dos movimentos feitos. 

- Execução do Teste: 

• A todo tempo que o usuário mexe no joystick para os eixos X e Y, através da tela
do DISPLAY OLED, é exibido o valor de X e Y de acordo com o movimento do usuário.  

##  Arquivos
- `projetos/Bitdoglab_joystick/Bitdoglab_joystick.c`: Código principal do projeto
- `projetos/Bitdoglab_joystick/CMakeLists.txt`: Arquivo CMake que contém as diretrizes do projeto e as bibliotecas utilizadas. 
- `projetos/LICENSE.txt`: Licença de distribuição. 


---
## 📜 Licença
MIT License
