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

#define ADC_TEMPERATURE_CHANNEL 4  
#define NUM_AMOSTRAS 15  // Número de leituras para média móvel

// Função para converter o valor lido do ADC para temperatura em Celsius
float adc_to_temperature_celsius(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);  // 12 bits -> 0 a 4095
    float voltage = adc_value * conversion_factor;
    float temperature_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temperature_celsius;
}

int main() {
    stdio_init_all();
    adc_init();

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

    ssd1306_draw_string(ssd, 0, 16, "   LEITURA DA ");
    ssd1306_draw_string(ssd, 0, 32, "TEMPERATURA EM C");
    render_on_display(ssd, &frame_area);
    sleep_ms(4000);

    adc_set_temp_sensor_enabled(true);  
    adc_select_input(ADC_TEMPERATURE_CHANNEL);  

    while (true) {
        uint32_t soma_adc = 0;

        // incrementa as leituras até chegar em 15 leituras e da um pequeno delay entre as amostras.
        for (int i = 0; i < NUM_AMOSTRAS; i++) {
            soma_adc += adc_read();
            sleep_ms(50);  // Pequeno delay entre amostras
        }

        // média móvel para melhoria da precisão e variações.
        uint16_t media_adc = soma_adc / NUM_AMOSTRAS;
        float temperatura = adc_to_temperature_celsius(media_adc);

        memset(ssd, 0, ssd1306_buffer_length);
        char texto[32];

        snprintf(texto, sizeof(texto), "   TEMPERATURA ");
        ssd1306_draw_string(ssd, 0, 16, texto);
        snprintf(texto, sizeof(texto), "      %.1f C", temperatura);
        ssd1306_draw_string(ssd, 0, 32, texto);
        render_on_display(ssd, &frame_area);

        sleep_ms(1000);  // Atualiza a cada segundo
    }

    return 0;
}
