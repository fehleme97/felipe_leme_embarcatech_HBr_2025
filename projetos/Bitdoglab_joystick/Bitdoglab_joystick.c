#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_i2c.h"
#include <string.h>
#include <stdlib.h>
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

int main() {
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

    ssd1306_draw_string(ssd, 0, 16, "MOVIMENTOS X E Y ");
    ssd1306_draw_string(ssd, 0, 32, "  DO JOYSTICKR ");
    render_on_display(ssd, &frame_area);
    sleep_ms(4000);

    memset(ssd, 0, ssd1306_buffer_length);
    ssd1306_draw_string(ssd, 0, 16, "   MANIPULE O ");
    ssd1306_draw_string(ssd, 0, 32, "    JOYSTICKR ");
    render_on_display(ssd, &frame_area);
    sleep_ms(2000);

    stdio_init_all();
    adc_init();
    adc_gpio_init(26); // ADC0 - Eixo X
    adc_gpio_init(27); // ADC1 - Eixo Y

    // Calibrar posição inicial como centro
    adc_select_input(0);
    uint16_t x_offset = adc_read();
    adc_select_input(1);
    uint16_t y_offset = adc_read();

    while (1) {
        adc_select_input(1);
        uint16_t adc_x_raw = adc_read();
        adc_select_input(0);
        uint16_t adc_y_raw = adc_read();

        int x = adc_x_raw - x_offset;
        int y = adc_y_raw - y_offset;

        memset(ssd, 0, ssd1306_buffer_length);
        char texto[32];

        snprintf(texto, sizeof(texto), "X: %4d", x);
        ssd1306_draw_string(ssd, 0, 8, texto);

        snprintf(texto, sizeof(texto), "Y: %4d", y);
        ssd1306_draw_string(ssd, 0, 40, texto);

        render_on_display(ssd, &frame_area);

        sleep_ms(300);
    }

    return 0;
}