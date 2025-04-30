#include <stdio.h>
#include "adc_temp.h"

int main() {
    uint16_t leitura_adc = 876;
    float temperatura = adc_to_celsius(leitura_adc);
    printf("Temperatura: %.2f °C\n", temperatura);
    return 0;
}

