# Controle de Motores com Botões e PWM - Raspberry Pi Pico W

## 📝 Descrição do Projeto
Este projeto permite controlar dois motores DC utilizando a Raspberry Pi Pico W com um driver de ponte H (como o TB6612 ou L298N). O controle é feito por dois botões:

- **Botão A (GPIO 5)**: gira o motor A para frente.
- **Botão B (GPIO 6)**: gira o motor B para trás.
- **Botões A + B pressionados simultaneamente**: altera o nível de velocidade dos motores em ciclos (25%, 50%, 100%).

A lógica utiliza PWM real para controle de velocidade, com alternância a cada clique duplo detectado, possibilitando o ajuste dinâmico durante a execução.

## 🎯 Objetivo
Demonstrar um controle simples e funcional de dois motores com direção e velocidade, utilizando entrada digital (botões) e PWM com duty cycle variável. O projeto é útil tanto para fins didáticos quanto para sistemas embarcados básicos com movimentação.

## 🔁 Comportamento Esperado
- O motor **A gira para frente** enquanto o botão A estiver pressionado.
- O motor **B gira para trás** enquanto o botão B estiver pressionado.
- **Pressionar ambos os botões simultaneamente** alterna o nível de velocidade entre:
  - 25% duty (baixa velocidade)
  - 50% duty (velocidade média)
  - 100% duty (velocidade máxima)
  - O ciclo reinicia após 3 cliques.

A velocidade afeta **ambos os motores** quando estão em operação.

## ⚙️ Estrutura do Projeto 📂
```
/motor_drive
├── motor_drive.c   # Código principal do controle de motores
├── CMakeLists.txt  # Script de build para o projeto
├── README.md       # Este arquivo
├── LICENSE.txt     # Licensa do projeto
```

## 📌 Diagrama de Conexões

| Componente        | GPIO Pico W |
|------------------|-------------|
| Botão A          | GPIO 5      |
| Botão B          | GPIO 6      |
| IN1A (motor A)   | GPIO 4      |
| IN2A (motor A)   | GPIO 9      |
| IN1B (motor B)   | GPIO 18     |
| IN2B (motor B)   | GPIO 19     |
| PWM A            | GPIO 8      |
| PWM B            | GPIO 16     |
| STBY (driver)    | GPIO 20     |

**Observação**: Os botões devem estar ligados ao GND com pull-up interno ativado no firmware.

---

## 🖥️ Requisitos

- Raspberry Pi Pico W (BitDogLab)
- Driver de motor compatível (ex: TB6612FNG, L298N, etc.)
- Dois motores DC
- 2 botões físicos
- Fonte de alimentação adequada aos motores
- SDK da Raspberry Pi Pico instalado
- `cmake`, `ninja`, e `arm-none-eabi-gcc`

---

## 🚀 Como compilar

1. **Clone o projeto e o SDK (caso não tenha):**
```
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

2. **Configure seu ambiente:**
```
export PICO_SDK_PATH=~/pico-sdk
```

3. **Compile o projeto** 
```
mkdir build
cd build
cmake .. -G Ninja
ninja
```

4. O arquivo .uf2 estará disponível na pasta build/, pronto para gravar na sua Raspberry Pi Pico W via USB.

## 📜 Licença
MIT License
