#include "pico/stdlib.h" // Biblioteca padrão do Pico (sleep_ms, etc.)
#include "hal_led.h"     // Interface HAL para controle do LED

int main() {
    stdio_init_all();    // Inicializa as saídas padrão (não estritamente necessário aqui)

    hal_led_init();      // Inicializa o LED

    while (true) {
        hal_led_toggle();  // Alterna o estado do LED (liga/desliga)
        sleep_ms(500);     // Aguarda 500 ms
    }
}
