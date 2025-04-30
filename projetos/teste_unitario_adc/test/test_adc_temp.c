#include <stdio.h>
#include "unity.h"
#include "adc_temp.h"
#include "pico/stdlib.h"


void test_adc_to_celsius_expected_27C(void) {
    uint16_t adc_val = 876;  // Aproximadamente 0.706 V
    float temperature = adc_to_celsius(adc_val);
    printf("Valor ADC: %u -> Temperatura calculada: %.2f °C\n", adc_val, temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 27.0, temperature);
}

int main(void) {

    stdio_init_all();  // Garante inicialização do USB para printf
    sleep_ms(10000);

    printf("=== Iniciando testes de temperatura com Unity ===\n");

    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius_expected_27C);
    return UNITY_END();
}
