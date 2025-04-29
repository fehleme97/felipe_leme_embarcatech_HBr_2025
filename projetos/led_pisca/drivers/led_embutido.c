#include "led_embutido.h"
#include "pico/cyw43_arch.h"

// Inicializa o sistema CYW43 necessário para controlar o LED
int led_embutido_init() {
    if (cyw43_arch_init()) {
        return -1; // Retorna erro se a inicialização falhar
    }
    return 0; // Sucesso
}

// Liga o LED
void led_embutido_on(void) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

// Desliga o LED
void led_embutido_off(void) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}
