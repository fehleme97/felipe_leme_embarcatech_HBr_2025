#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "task.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12
#define BUZZER_PIN 21
#define BUZZER_FREQUENCY 60

// Definir cor(es) do led RGB
static inline void set_rgb_led(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(
        &config,
        clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));  // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Inicializa os indicadores de acão
void act_init(void) {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    pwm_init_buzzer(BUZZER_PIN);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);
}

// Manda um beep ao buzzer
void buzzer_beep(int ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(BUZZER_PIN, 2048);

    // Temporização
    sleep_ms(ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(BUZZER_PIN, 0);

    // Pausa entre os beeps
    sleep_ms(ms);  // Pausa de 100ms
}
