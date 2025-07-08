#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include <string.h>
#include <stdio.h>

// === Pinos ===
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

#define ADC_TEMPERATURE_CHANNEL  4
#define LED_R 13
#define LED_G 11
#define LED_B 12

// === Define a constate de amostrar do canal ADC ===
#define NUM_AMOSTRAS  15

// === Configurações iniciais e globais do Display ===
ssd1306_t oled;
uint8_t ssd[ssd1306_buffer_length];
struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

// === Variáveis globais ===
float temperatura = 0.0f;
bool alerta_ativo = false;

// === Conversão de ADC para Celsius ===
float adc_to_temperature_celsius(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

// === Controle do LED RGB ===
void set_rgb(bool r, bool g, bool b) {
    gpio_put(LED_R, r);
    gpio_put(LED_G, g);
    gpio_put(LED_B, b);
}

// === Tarefa: leitura e exibição da temperatura ou alerta ===
void task_display(void *p) {
    char texto[32];

    while (1) {
        // Leitura da temperatura
        uint32_t soma_adc = 0;
        for (int i = 0; i < NUM_AMOSTRAS; i++) {
            soma_adc += adc_read();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        uint16_t media_adc = soma_adc / NUM_AMOSTRAS;
        temperatura = adc_to_temperature_celsius(media_adc);

        memset(ssd, 0, ssd1306_buffer_length);

        if (temperatura > 40.0f) {
            alerta_ativo = true;
            ssd1306_draw_string(ssd, 0, 0, "     ALERTA!!");
            ssd1306_draw_string(ssd, 0, 32, "TEMPERATURA ALTA");
        } else {
            alerta_ativo = false;
            ssd1306_draw_string(ssd, 0, 16, "   TEMPERATURA");
            snprintf(texto, sizeof(texto), "     %d°C", (int)temperatura);
            ssd1306_draw_string(ssd, 0, 32, texto);
        }

        render_on_display(ssd, &frame_area);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// === Tarefa: controle do LED RGB ===
void task_led(void *p) {
    while (1) {
        if (alerta_ativo) {
            set_rgb(true, false, false);  // Vermelho fixo
        } else if (temperatura > 33.0f && temperatura <= 39.0f) {
            set_rgb(true, true, false);   // Amarelo
        } else {
            set_rgb(false, true, false);  // Verde
        }

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

// === main ===
int main() {
    stdio_init_all();

    // Inicialização do ADC
    adc_init();
    adc_select_input(ADC_TEMPERATURE_CHANNEL);
    adc_set_temp_sensor_enabled(true);

    // Inicializa I2C e OLED
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    calculate_render_area_buffer_length(&frame_area);

    // Inicializa LEDs
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);
    set_rgb(false, false, false);

    // Mensagem de inicialização
    ssd1306_draw_string(ssd, 0, 16, "    SISTEMA");
    ssd1306_draw_string(ssd, 0, 32, "   INICIANDO...");
    render_on_display(ssd, &frame_area);
    sleep_ms(3000);

    // Criação das tarefas
    xTaskCreate(task_display, "Display", 768, NULL, 1, NULL);
    xTaskCreate(task_led, "LED", 512, NULL, 2, NULL);

    vTaskStartScheduler();
    while (1) {}
}
