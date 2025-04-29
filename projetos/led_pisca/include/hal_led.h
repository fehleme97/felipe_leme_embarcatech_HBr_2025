#ifndef _HAL_LED_H
#define _HAL_LED_H

// Inicializa o LED
void hal_led_init(void);

// Alterna o estado do LED (liga se desligado, desliga se ligado)
void hal_led_toggle(void);

#endif // _HAL_LED_H
