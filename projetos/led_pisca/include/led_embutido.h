#ifndef _LED_EMBUTIDO_H
#define _LED_EMBUTIDO_H

// Inicializa o driver do LED embutido
// Retorna 0 em sucesso, -1 em erro
int led_embutido_init(void);

// Liga o LED embutido
void led_embutido_on(void);

// Desliga o LED embutido
void led_embutido_off(void);

#endif // _LED_EMBUTIDO_H
