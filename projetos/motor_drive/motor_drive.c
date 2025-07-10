#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define os pinos
#define INA1 4
#define INA2 9
#define INB1 18
#define INB2 19
#define STBY 20
#define PWM_DRIVERA 8
#define PWM_DRIVERB 16
#define BOTAO_A 5
#define BOTAO_B 6

// Variável global para armazenar o duty atual (modo 0,1,2)
uint8_t velocidade_nivel = 0;             // Índice do nível de velocidade atual: 0 = 25%, 1 = 50%, 2 = 100%
uint16_t duty_values[] = {64, 128, 255};  // Valores de duty cycle correspondentes aos níveis de velocidade

uint pwm_slice_a, pwm_slice_b;            // Slices PWM atribuídos aos pinos PWM

// Setagem das pinagens para controle do motor e PWM dos motores
void setup_motor_pins() {
    gpio_init(INA1); gpio_set_dir(INA1, GPIO_OUT);
    gpio_init(INA2); gpio_set_dir(INA2, GPIO_OUT);
    gpio_init(INB1); gpio_set_dir(INB1, GPIO_OUT);
    gpio_init(INB2); gpio_set_dir(INB2, GPIO_OUT);
    gpio_init(STBY); gpio_set_dir(STBY, GPIO_OUT); // Pinagem que ativa o driver.

    // Configura os pinos de PWM com função alternativa
    gpio_set_function(PWM_DRIVERA, GPIO_FUNC_PWM);
    gpio_set_function(PWM_DRIVERB, GPIO_FUNC_PWM);

    // Obtém os slices de PWM para os pinos
    pwm_slice_a = pwm_gpio_to_slice_num(PWM_DRIVERA);
    pwm_slice_b = pwm_gpio_to_slice_num(PWM_DRIVERB);

    // Define o valor máximo de contagem do PWM (100% = 255)
    pwm_set_wrap(pwm_slice_a, 255); 
    pwm_set_wrap(pwm_slice_b, 255);

    pwm_set_enabled(pwm_slice_a, true);
    pwm_set_enabled(pwm_slice_b, true);
}

// Setagem das pinagens dos botões com pull-up
void setup_botoes() {
    gpio_init(BOTAO_A); gpio_set_dir(BOTAO_A, GPIO_IN); gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B); gpio_set_dir(BOTAO_B, GPIO_IN); gpio_pull_up(BOTAO_B);
}

// Setagem do duty cicle para cada motor 
void aplicar_pwm() {
    uint16_t duty = duty_values[velocidade_nivel];
    pwm_set_chan_level(pwm_slice_a, PWM_CHAN_A, duty);
    pwm_set_chan_level(pwm_slice_b, PWM_CHAN_A, duty);
}

// Gira motor A para frente
void gira_frente(void) {
    gpio_put(INA1, 1);
    gpio_put(INA2, 0);
    gpio_put(STBY, 1);
    aplicar_pwm();
}

// Gira motor B para trás
void gira_tras(void) {
    gpio_put(INB1, 0);
    gpio_put(INB2, 1);
    gpio_put(STBY, 1);
    aplicar_pwm();
}

// Para ambos os motores
void parar_motor(void) {
    gpio_put(INA1, 0);
    gpio_put(INA2, 0);
    gpio_put(INB1, 0);
    gpio_put(INB2, 0);
    gpio_put(STBY, 0);
    pwm_set_chan_level(pwm_slice_a, PWM_CHAN_A, 0);
    pwm_set_chan_level(pwm_slice_b, PWM_CHAN_A, 0);
}

int main() {
    stdio_init_all();
    setup_motor_pins();
    setup_botoes();

    bool ultimo_estado_ab = false;                      // Guarda o último estado de A+B pressionados
    absolute_time_t ultimo_click = get_absolute_time(); // Tempo do último clique conjunto

    while (true) {
        bool a = !gpio_get(BOTAO_A);                    // Invertido porque usamos pull-up (pressionado = 0 lógico)
        bool b = !gpio_get(BOTAO_B);

        // Detecta clique simultâneo A+B
        if (a && b && !ultimo_estado_ab) {
            // Debounce simples: espera 300 ms entre cliques
            if (absolute_time_diff_us(ultimo_click, get_absolute_time()) > 300000) {
                velocidade_nivel = (velocidade_nivel + 1) % 3;  // alterna entre 0,1,2
                aplicar_pwm(); // Aplica a nova velocidade
                ultimo_click = get_absolute_time();
            }
        }

        ultimo_estado_ab = a && b;

        if (a && !b) {
            gira_frente();
        } else if (!a && b) {
            gira_tras();
        } else if (!a && !b) {
            parar_motor();
        }

        sleep_ms(50);
    }

    return 0;
}
