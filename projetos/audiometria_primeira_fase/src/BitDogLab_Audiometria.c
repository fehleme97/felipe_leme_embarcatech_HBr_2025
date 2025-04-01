#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_i2c.h"
#include <time.h> 


// Definições dos pinos
#define BUZZER_A_PIN 21
#define BUZZER_B_PIN 10
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define LED_GREEN_PIN 11
#define LED_RED_PIN 13

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Frequências e intensidades
const uint16_t frequencies[] = {250, 500, 1000, 2000, 4000, 8000};
const uint8_t intensities[] = {10, 30, 50, 70, 90};
const uint num_frequencies = sizeof(frequencies) / sizeof(frequencies[0]);
const uint num_intensities = sizeof(intensities) / sizeof(intensities[0]);

// Configura o PWM nos buzzers
void setup_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice_num, 255);
    pwm_set_enabled(slice_num, true);
}

// Gera um tom no buzzer
void play_tone(uint pin, uint16_t frequency, uint8_t intensity) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice_num, 125000000 / (frequency * 256));
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(pin), intensity);
}

// Para o som do buzzer
void stop_tone(uint pin) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(pin), 0);
}

// Função principal
int main() {
    stdio_init_all();
    srand(time(NULL));
    srand(to_us_since_boot(get_absolute_time()));

    // Inicialização 
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    // Configura buzzer, botões e LEDs
    setup_buzzer(BUZZER_A_PIN);
    setup_buzzer(BUZZER_B_PIN);
    gpio_init(BUTTON_A_PIN); gpio_set_dir(BUTTON_A_PIN, GPIO_IN); gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN); gpio_set_dir(BUTTON_B_PIN, GPIO_IN); gpio_pull_up(BUTTON_B_PIN);
    gpio_init(LED_GREEN_PIN); gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_init(LED_RED_PIN); gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    
    // Limpar
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Mensagem inicial
    ssd1306_draw_string(ssd, 0, 16, "   Exame de  ");
    ssd1306_draw_string(ssd, 0, 32, "  Audiometria -");
    render_on_display(ssd, &frame_area);
    sleep_ms(10000);
    
    ssd1306_draw_string(ssd, 0, 16, "Pressione A e B");
    ssd1306_draw_string(ssd, 0, 32, "para iniciar");
    ssd1306_draw_string(ssd, 0, 48, "o teste.");
    render_on_display(ssd, &frame_area);

    while (true) {
        while (gpio_get(BUTTON_A_PIN) || gpio_get(BUTTON_B_PIN));
        sleep_ms(1000);

        // Exibe a mensagem "O teste vai começar"
        memset(ssd, 0, ssd1306_buffer_length);
        ssd1306_draw_string(ssd, 0, 16, "    O teste   ");
        ssd1306_draw_string(ssd, 0, 32, "comeca em 4 Seg.");
        render_on_display(ssd, &frame_area);
        sleep_ms(5000);  // Aguarda 5 segundos para o usuário ver a mensagem    

        uint8_t correct_responses = 0;
        uint8_t total_tests = 10;
        uint8_t errors[total_tests][3];
        uint8_t error_count = 0;

        for (uint8_t i = 0; i < total_tests; i++) {
            uint16_t freq = frequencies[rand() % num_frequencies];
            uint8_t intensity = intensities[rand() % num_intensities];
            uint8_t buzzer = rand() % 2;

            play_tone(buzzer ? BUZZER_B_PIN : BUZZER_A_PIN, freq, intensity);
            printf("Teste %d: Frequência=%d Hz, Intensidade=%d%%, Buzzer=%s\n", i + 1, freq, intensity, buzzer ? "B" : "A");

            bool answered = false;
            while (!answered) {
                if (!gpio_get(BUTTON_A_PIN)) {
                    answered = true;
                    if (buzzer == 0) {
                        gpio_put(LED_GREEN_PIN, 1);
                        correct_responses++;
                        printf("Correto!\n");
                    } else {
                        gpio_put(LED_RED_PIN, 1);
                        errors[error_count][0] = freq;
                        errors[error_count][1] = intensity;
                        errors[error_count++][2] = buzzer;
                        printf("Errado!\n");
                    }
                }
                if (!gpio_get(BUTTON_B_PIN)) {
                    answered = true;
                    if (buzzer == 1) {
                        gpio_put(LED_GREEN_PIN, 1);
                        correct_responses++;
                        printf("Correto!\n");
                    } else {
                        gpio_put(LED_RED_PIN, 1);
                        errors[error_count][0] = freq;
                        errors[error_count][1] = intensity;
                        errors[error_count++][2] = buzzer;
                        printf("Errado!\n");
                    }
                }
                sleep_ms(10);
            }

            stop_tone(BUZZER_A_PIN);
            stop_tone(BUZZER_B_PIN);
            gpio_put(LED_GREEN_PIN, 0);
            gpio_put(LED_RED_PIN, 0);
            sleep_ms(1000);
        }

        // Exibe no display o resultado final
        memset(ssd, 0, ssd1306_buffer_length);
        char result[32];
        snprintf(result, sizeof(result), "Acertos: %d", correct_responses);
        ssd1306_draw_string(ssd, 0, 16, result);
        if (error_count > 0) {
            snprintf(result, sizeof(result), "Erros: %d", error_count);
            ssd1306_draw_string(ssd, 0, 32, result);
        }
        render_on_display(ssd, &frame_area);
        sleep_ms(3000);  

        //Exibe os resultados em um SerialMonitor 
        printf("Teste concluído! Acertos: %d/%d\n", correct_responses, total_tests);
        if (error_count > 0) {
            printf("Erros:\n");
            for (uint8_t i = 0; i < error_count; i++) {
                printf("Freq: %d Hz, Intensidade: %d%%, Buzzer: %s\n", errors[i][0], errors[i][1], errors[i][2] ? "B" : "A");
            }
        }
        sleep_ms(3000);

        // Exibe a mensagem de reiniciar ou sair
        memset(ssd, 0, ssd1306_buffer_length);
        ssd1306_draw_string(ssd, 0, 0, " Aperte 2X A");
        ssd1306_draw_string(ssd, 0, 16, " para reiniciar");
        ssd1306_draw_string(ssd, 0, 32, " Aperte 2X B");
        ssd1306_draw_string(ssd, 0, 48, " para sair. ");
        render_on_display(ssd, &frame_area);
        sleep_ms(3000);  // Exibe por 3 segundos

        uint8_t button_a_count = 0, button_b_count = 0;
        
        // Aguarda até que o botão A ou B seja pressionado duas vezes
        while (true) {
            if (!gpio_get(BUTTON_A_PIN)) {
                button_a_count++;
                sleep_ms(200);
            }
            if (!gpio_get(BUTTON_B_PIN)) {
                button_b_count++;
                sleep_ms(200);
            }

            // Se pressionar A duas vezes, reinicia o exame
            if (button_a_count == 2) {
                // Limpa a tela e exibe a mensagem de iniciar novamente
                memset(ssd, 0, ssd1306_buffer_length);
                ssd1306_draw_string(ssd, 0, 16, "Pressione A e B");
                ssd1306_draw_string(ssd, 0, 32, "para iniciar");
                ssd1306_draw_string(ssd, 0, 48, "o teste.");
                render_on_display(ssd, &frame_area);
                sleep_ms(3000);  // Exibe por 3 segundos
                break;
            }

            // Se pressionar B duas vezes, termina o programa
            if (button_b_count == 2) return 0;
        }
    }
}


