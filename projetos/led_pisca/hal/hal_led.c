#include "hal_led.h"
#include "led_embutido.h"
#include "pico/stdlib.h"
#include <stdbool.h>


// Inicializa o LED usando a função do driver de hardware
void hal_led_init() {
    led_embutido_init();
}

// Alterna o estado do LED
void hal_led_toggle() {
    static bool estado = false; // Estado atual do LED (false = desligado, true = ligado)

    if (estado) {
        led_embutido_off(); // Se está ligado, desliga
    } else {
        led_embutido_on();  // Se está desligado, liga
    }

    estado = !estado; // Inverte o estado para a próxima chamada
}
