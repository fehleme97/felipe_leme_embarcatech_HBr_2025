#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_i2c.h"
#include <string.h>
#include <stdlib.h>
#include "hardware/i2c.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

const uint DEBOUNCE_DELAY_MS = 200;

// Variáveis globais
volatile bool button_a_flag = false;
volatile bool button_b_flag = false;
volatile int button_b_count = 0;

int main() {
    stdio_init_all();
    
    // Inicialização do display OLED
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    calculate_render_area_buffer_length(&frame_area);

    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    ssd1306_draw_string(ssd, 0, 16, "   PRESSIONE A ");
    ssd1306_draw_string(ssd, 0, 32, "  PARA COMECAR ");
    render_on_display(ssd, &frame_area);

    char texto[32];  // Buffer para exibir textos no display

    int counter = 0;
    bool counting = false;
    absolute_time_t last_time = get_absolute_time();
    absolute_time_t last_b_press = get_absolute_time();
    absolute_time_t last_a_press = get_absolute_time();

    while (1) {

        // Detecta borda de descida botão A
        static bool last_button_a = true;
        bool current_button_a = gpio_get(BUTTON_A_PIN);
        if (!current_button_a && last_button_a) {
            // Debounce botão A
            if (absolute_time_diff_us(last_a_press, get_absolute_time()) > DEBOUNCE_DELAY_MS * 1000) {
                button_a_flag = true;
                last_a_press = get_absolute_time();
            }
        }
        last_button_a = current_button_a;

        // Detecta borda de descida botão B
        static bool last_button_b = true;
        bool current_button_b = gpio_get(BUTTON_B_PIN);
        if (!current_button_b && last_button_b) {
            // Debounce botão B
            if (absolute_time_diff_us(last_b_press, get_absolute_time()) > DEBOUNCE_DELAY_MS * 1000) {
                button_b_flag = true;
                last_b_press = get_absolute_time();
            }
        }
        last_button_b = current_button_b;

        // Lógica botão A - inicia nova contagem
        if (button_a_flag) {
            button_a_flag = false;
            counter = 9;
            counting = true;
            button_b_count = 0;
            last_time = get_absolute_time();
        
            memset(ssd, 0, ssd1306_buffer_length);
            snprintf(texto, sizeof(texto), "    Contador");
            ssd1306_draw_string(ssd, 0, 16, texto);
            snprintf(texto, sizeof(texto), "  iniciado: %d", counter);
            ssd1306_draw_string(ssd, 0, 32, texto);
            render_on_display(ssd, &frame_area);
        
            printf("\nContador iniciado: 9\n");
        }

        // Lógica botão B (só ativa se estiver contando)
        if (button_b_flag) {
            button_b_flag = false;
            if (counting) {
                button_b_count++;

               // printf("Botão B pressionado! Total: %d\n", button_b_count); //TESTE PARA SERIAL MONITOR
            }
        }

        // Contagem regressiva
        if (counting) {
            absolute_time_t now = get_absolute_time();
            if (absolute_time_diff_us(last_time, now) >= 1000000) {
                last_time = now;
                if (counter > 0) {
                    counter--;
        
                    memset(ssd, 0, ssd1306_buffer_length);
                    snprintf(texto, sizeof(texto), "Contando = %d", counter);
                    ssd1306_draw_string(ssd, 0, 16, texto);
                    render_on_display(ssd, &frame_area);
                    snprintf(texto, sizeof(texto), "B Apertado = %d", button_b_count);
                    ssd1306_draw_string(ssd, 0, 48, texto);
                    render_on_display(ssd, &frame_area);
        
                    // printf("Contador: %d\n", counter); //TESTE PARA SERIAL MONITOR

                } else {
                    counting = false;
        
                    memset(ssd, 0, ssd1306_buffer_length);
                    snprintf(texto, sizeof(texto), "       FIM! ");
                    ssd1306_draw_string(ssd, 0, 16, texto);
                    snprintf(texto, sizeof(texto), " B APERTADO: %dX", button_b_count);
                    ssd1306_draw_string(ssd, 0, 48, texto);
                    render_on_display(ssd, &frame_area);
                    sleep_ms(6000); //Delay de 5s

                    memset(ssd, 0, ssd1306_buffer_length);
                    snprintf(texto, sizeof(texto), "    APERTE A");
                    ssd1306_draw_string(ssd, 0, 16, texto);
                    snprintf(texto, sizeof(texto), " PARA REINICIAR");
                    ssd1306_draw_string(ssd, 0, 32, texto);
                    snprintf(texto, sizeof(texto), "   A CONTAGEM");
                    ssd1306_draw_string(ssd, 0, 48, texto);
                    render_on_display(ssd, &frame_area);
               

                    // printf("FIM DA CONTAGEM!\n"); //TESTE PARA SERIAL MONITOR
                }

            }
        }

        tight_loop_contents();
    }

    return 0;
}
