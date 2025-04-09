## PROJETO 1

**EXEMPLO DE PREENCHIMENTO**

# Projeto contador decrescente por interrupção 
Este projeto implementa uma versão digital do teste de audiometria, que visa diagnosticar pessoas com deficiências auditivas em determinadas frequências e intensidade.

## Objetivo
Criar um sistema para triagem de pessoas com deficiência auditiva através de:
- Display OLED;
- Botões para interação do usuário com os sinais sonoros gerados;
- Buzzers para gerar tons e intensidades aleatórias entre 250 a 8kHz;
- Led RGB para feedback visual; 

##  Lista de materiais: 

| Componente           | Conexão na BitDogLab |
|----------------------|----------------------|
| BitDogLab (RP2040)   | -                    |
| Display OLED I2C     | SDA: GPIO14 / SCL: GPIO15 |
| LED_RAGB             | GPIO11 / GPIO13      |
| SW2                  | GP07                 |
| SW3                  | GP09                 |
| BZ1                  | GP21                 |
| BZ1                  | GP14                 |


Execução
Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM).


Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make).


Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo).


Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2).


A Pico reiniciará automaticamente e começará a executar o código.


O teste de audiometria será iniciado e apresentará as instruções ao usuário no display OLED.

## Lógica

- Inicialização:  

• O sistema inicia com uma mensagem no display OLED instruindo o usuário a 
pressionar os botões A e B para começar o teste. 

• Após o início, o sistema exibe uma mensagem indicando que o teste começará 
em 4 segundos. 

- Execução do Teste: 

• O teste consiste em 10 rodadas, onde em cada rodada: 
Uma frequência aleatória (250 Hz, 500 Hz, 1000 Hz, 2000 Hz, 4000 Hz, ou 8000 
Hz) e uma intensidade aleatória (10%, 30%, 50%, 70%, ou 90%) são selecionadas. 

• O som é emitido por um dos buzzers (A ou B), escolhido aleatoriamente. 

• O usuário deve pressionar o botão correspondente ao buzzer que está emitindo 
o som. 

• Se a resposta estiver correta, o LED verde acende. Caso contrário, o LED 
vermelho acende e o erro é registrado. 

- Finalização do Teste: 

• Após as 10 rodadas, o sistema exibe no display OLED o número de acertos e 
erros. 

• Os erros são detalhados no Serial Monitor, mostrando a frequência, 
intensidade e o buzzer correspondente a cada erro. 

• O usuário pode optar por reiniciar o teste (pressionando o botão A duas vezes) 
ou sair do programa (pressionando o botão B duas vezes).

##  Arquivos
- `src/BitDogLab_Audiometria.c`: Código principal do projeto
- `docs/U7T_ FLCdS.pdf`: Documentação
- `projetos/LICENSE.txt`: Licença de distribuição. 

## 🖼️ Imagens do Projeto
### Matriz de LEDs durante execução
![BitDogLab_led](./assets/BitDogLab_led.jpg)

---
## 📜 Licença
MIT License
