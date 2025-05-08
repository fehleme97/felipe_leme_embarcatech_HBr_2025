
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: **Felipe Leme Correa da Silva*

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 09 de maio de 2025

---

## Simulação da Galton Board

## 📝 O projeto
Este projeto implementa um Digital Twins de uma Galton Board, que consiste em um dispositivo composto por obstáculos esféricos estruturado em linhas triangulares e esferas que decorrem o percurso, forçando as bolas a cairem em caminhos diversos e demonstra visualmente a distribuição normal. O projeto foi inteiramente feito através do Display OLED e utilizado a técnica de algoritmo de Bresenham. 

Através do botão A, o usuário pode alterar para ver o Histograma gerado pelas bolinhas que foram disponibilizados nas canaletas de armazenamento, sendo 8 canaletas no total, além de contabilizar quantas bolinhas foram geradas em tempo real. 

Pelo botão B, o usuário incrementa 50 bolinhas aleatórias geradas simultaneamente. 

Durante o curso, foram utilizados bibliotecas de exemplos do ssd1306, disponível em: https://github.com/BitDogLab/BitDogLab-C/blob/main/display_oled/display_oled.c

## 🎯 Objetivo
Fazer uma simulação virtual da Galton Board através do display OLED de 128x64. 
- Display OLED;
- Botões para interação do usuário; 

## 🛠️ Lista de materiais: 

| Componente           | Conexão na BitDogLab |
|----------------------|----------------------|
| BitDogLab (RP2040)   | -                    |
| Display OLED I2C     | SDA: GPIO14 / SCL: GPIO15 |
| SW2                  | GP05                 |
| SW3                  | GP06                 |


## 🖥️ Execução
Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM).


Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make).


Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo).


Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2).


A Pico reiniciará automaticamente e começará a executar o código.


As instruções ao usuário serão impressas no display OLED e posterior o contador irá começar.

## 🚀 Lógica

- Inicialização:
• O sistema inicializa o display OLED, os botões (A e B) e a simulação da Galton Board.
• O display começa mostrando a simulação em tempo real com uma bolinha sendo lançada automaticamente a cada 2 segundos.

- Execução da Simulação:
• A cada 2 segundos, uma nova bolinha é lançada automaticamente no ponto inicial do tabuleiro.
• As bolinhas percorrem o campo de obstáculos, desviando aleatoriamente para cima ou para baixo ao colidir com um obstáculo.
• Ao alcançar o fim do tabuleiro (lado direito), a bolinha é direcionada para uma das 8 canaletas verticais, registrando sua posição.

- Interação com o Usuário:
• O botão A alterna entre dois modos de visualização: simulação em tempo real e histograma.
• No modo histograma, são exibidas as distribuições das bolinhas nas canaletas e o número total de bolinhas geradas.
• O botão B permite lançar 50 bolinhas de uma vez, simulando um pico de geração. O contador total de bolinhas é atualizado corretamente.

- Contador de Bolinhas:
• O número total de bolinhas geradas (automáticas e por botão B) é exibido no canto superior direito durante o modo histograma.
• A lógica garante que cada bolinha gerada seja contabilizada corretamente, independente do momento de sua criação.

##  Arquivos
- `projetos/contador_decrescente/contador_decrescente.c`: Código principal do projeto
- `projetos/contador_decrescente/CMakeLists.txt`: Arquivo CMake que contém as diretrizes do projeto e as bibliotecas utilizadas. 
- `projetos/LICENSE`: Licença de distribuição. 


---
## 📜 Licença
MIT License

---

