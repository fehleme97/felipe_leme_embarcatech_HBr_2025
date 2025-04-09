#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "ssd1306.h"
#include "font.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Variáveis globais
volatile int counter = 0;
volatile int button_b_presses = 0;
volatile bool counting_active = false;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

// Handler de interrupção para o Botão A
void button_a_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A_PIN) {
        button_a_pressed = true;
    }
}

// Handler de interrupção para o Botão B
void button_b_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_B_PIN && counting_active) {
        button_b_pressed = true;
    }
}

// Função para inicializar o display OLED
void init_display(SSD1306_t *dev) {
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    ssd1306_init(dev, 128, 64, 0x3C, i2c_default);
    ssd1306_clear_screen(dev, false);
}

// Função para atualizar o display
void update_display(SSD1306_t *dev) {
    char line1[20];
    char line2[20];
    
    ssd1306_clear_screen(dev, false);
    
    snprintf(line1, sizeof(line1), "Contador: %d", counter);
    ssd1306_display_text(dev, 0, line1, strlen(line1), false);
    
    snprintf(line2, sizeof(line2), "Botao B: %d", button_b_presses);
    ssd1306_display_text(dev, 1, line2, strlen(line2), false);
}

int main() {
    stdio_init_all();
    
    // Inicialização do display OLED
    SSD1306_t dev;
    init_display(&dev);
    
    // Configuração dos botões
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);
    
    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_a_isr);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_add_raw_irq_handler(BUTTON_B_PIN, button_b_isr);
    
    // Mensagem inicial
    ssd1306_display_text(&dev, 0, "Pressione A", 11, false);
    ssd1306_display_text(&dev, 1, "para comecar", 12, false);
    
    absolute_time_t last_count_time = nil_time;
    
    while (1) {
        // Verifica se o Botão A foi pressionado
        if (button_a_pressed) {
            button_a_pressed = false;
            counter = 9;
            button_b_presses = 0;
            counting_active = true;
            update_display(&dev);
            last_count_time = get_absolute_time();
        }
        
        // Verifica se o Botão B foi pressionado durante a contagem ativa
        if (button_b_pressed && counting_active) {
            button_b_pressed = false;
            button_b_presses++;
            update_display(&dev);
        }
        
        // Lógica da contagem regressiva
        if (counting_active) {
            absolute_time_t now = get_absolute_time();
            if (absolute_time_diff_us(last_count_time, now) >= 1000000) { // 1 segundo
                last_count_time = now;
                
                if (counter > 0) {
                    counter--;
                    update_display(&dev);
                } else {
                    counting_active = false; // Congela quando chega a zero
                }
            }
        }
        
        tight_loop_contents();
    }
    
    return 0;
}