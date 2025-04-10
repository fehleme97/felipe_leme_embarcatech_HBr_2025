## PROJETO 2

# Projeto de contador decrescente - Segunda Fase - Embarcatech
Este projeto implementa um contador de tempo decrescente que vai de 9 a 0, e a cada vez que o botão A é pressionado a contagem zera. Além de que, ao aperta o botão B é incrementando quantas vezes o mesmo foi pressionado na contagem de 9 a 0. Após finalizar a contagem, o processo é congelado e imprimir na tela a quantidade de vezes o botão B foi pressionado. 

## Objetivo
Fazer um contador de tempo decrescente e que imprima quantas vezes o botão B foi pressionado ao longo do tempo, com:
- Display OLED;
- Botões para interação do usuário; 

##  Lista de materiais: 

| Componente           | Conexão na BitDogLab |
|----------------------|----------------------|
| BitDogLab (RP2040)   | -                    |
| Display OLED I2C     | SDA: GPIO14 / SCL: GPIO15 |
| SW2                  | GP05                 |
| SW3                  | GP06                 |


Execução
Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM).


Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make).


Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo).


Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2).


A Pico reiniciará automaticamente e começará a executar o código.


As instruções ao usuário serão impressas no display OLED e posterior o contador irá começar.

## Lógica

- Inicialização:  

• O sistema inicia com uma mensagem no display OLED instruindo o usuário a 
pressionar o botão A para começar o contador. 

• Após o início, o sistema exibe uma mensagem indicando que o contado iniciou a contagem e imprimir o tempo a cada 1s. 

- Execução do Teste: 

• Ao longo da contagem o usuário tem a opção de optar por reiniciar o contador através do botão A 
ou pressionar o botão B, para incrementar a quantidade de vezes que durante a contagem o botão
foi pressionado.  

• Após o término da contagem, o programa congela as interações e imprimir a quantidade de vezes
que o botão B foi pressionado e posteriormente, da a opção do usuário reiniciar o contador
através do botão A. 

##  Arquivos
- `projetos/contador_decrescente/contador_decrescente.c`: Código principal do projeto
- `projetos/contador_decrescente/CMakeLists.txt`: Arquivo CMake que contém as diretrizes do projeto e as bibliotecas utilizadas. 
- `projetos/LICENSE.txt`: Licença de distribuição. 


---
## 📜 Licença
MIT License
