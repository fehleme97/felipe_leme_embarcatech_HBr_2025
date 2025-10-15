#include "max30102.h"

#include <math.h>
#include <stdio.h>

#include "pico/stdlib.h"

// Escrita do sensor MAX30102
static inline void max30102_write(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c1, MAX30102_ADDR, buf, 2,
                       false);  // Libera escrita
}

// Leitura do sensor
static inline uint8_t max30102_read(uint8_t reg) {
    uint8_t val;
    i2c_write_blocking(i2c1, MAX30102_ADDR, &reg, 1,
                       true);  // Bloqueia escrita
    i2c_read_blocking(i2c1, MAX30102_ADDR, &val, 1,
                      false);  // Libera leitura
    return val;
}

// Inicialização do sensor MAX30102
void max30102_init(void) {
    // Soft reset
    max30102_write(REG_MODE_CONFIG, 0x40);
    sleep_ms(100);

    // Limpar ponteiros do buffer FIFO
    max30102_write(REG_FIFO_WR_PTR, 0x00);   // Ponteiro Write
    max30102_write(REG_FIFO_OVF_CNT, 0x00);  // Ponteiro Overflow
    max30102_write(REG_FIFO_RD_PTR, 0x00);   // Ponteiro Read

    // Configuração FIFO:
    //   - média de amostras = 4
    //   - rollover habilitado
    //   - limite de amostras = 17
    max30102_write(REG_FIFO_CONFIG, (0b010 << 5) | (1 << 4) | 0x11);

    // Configuração de SpO2:
    //   - resolução do ADC: 4096 nA
    //   - taxa de amostragem de 100 Hz
    //   - 411 µs (18 bits de amplitude)
    max30102_write(REG_SPO2_CONFIG, (0b01 << 5) | (0b011 << 2) | 0b11);

    // Amplitudes de operação dos LEDs (~6.4 mA -> 0x24)
    max30102_write(REG_LED1_PA, 0x24);  // RED
    max30102_write(REG_LED2_PA, 0x24);  // IR

    // Habilitação do modo SpO2 (RED e IR)
    //   - 0x03 = modo SpO₂
    max30102_write(REG_MODE_CONFIG, 0x03);
}

// Leitura de amostras LED/IR
bool max30102_read_sample(uint32_t *red, uint32_t *ir) {
    uint8_t wr = max30102_read(REG_FIFO_WR_PTR);
    uint8_t rd = max30102_read(REG_FIFO_RD_PTR);
    if (wr == rd) return false;  // Sem amostras não lidas

    uint8_t data[6];
    uint8_t reg = REG_FIFO_DATA;
    i2c_write_blocking(i2c1, MAX30102_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c1, MAX30102_ADDR, data, 6, false);

    // 3 bytes por amostra -> Mascarar para 18 bits
    *red = ((uint32_t)data[0] << 16 | data[1] << 8 | data[2]) & 0x3FFFF;
    *ir = ((uint32_t)data[3] << 16 | data[4] << 8 | data[5]) & 0x3FFFF;
    return true;  // Amostras não lidas (há dados em standby)
}

#define SAMPLE_SIZE 100            // 100 amostras para RED e IR
uint32_t red_buffer[SAMPLE_SIZE];  // Buffer de RED
uint32_t ir_buffer[SAMPLE_SIZE];   // Buffer de IR

// Cálculo de oxigenação (SPO2)
float calculate_spo2(uint32_t *red, uint32_t *ir) {
    float red_ac = 0, ir_ac = 0;
    float red_dc = 0, ir_dc = 0;

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        red_dc += red[i];
        ir_dc += ir[i];
    }

    red_dc /= SAMPLE_SIZE;
    ir_dc /= SAMPLE_SIZE;

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        red_ac += fabsf(red[i] - red_dc);
        ir_ac += fabsf(ir[i] - ir_dc);
    }

    red_ac /= SAMPLE_SIZE;
    ir_ac /= SAMPLE_SIZE;

    float ratio = (red_ac / red_dc) / (ir_ac / ir_dc);
    float spo2 = 110.0f - 25.0f * ratio;  // Fórmula empírica

    if (spo2 > 100.0f) spo2 = 100.0f;
    if (spo2 < 0.0f) spo2 = 0.0f;
    return spo2;
}

// Cálculo de frequência cardíaca
float calculate_bpm(uint32_t *ir, float sample_rate_hz) {
    int peak_count = 0;
    for (int i = 1; i < SAMPLE_SIZE - 1; i++) {
        if (ir[i] > ir[i - 1] && ir[i] > ir[i + 1] && ir[i] > 10000)
            peak_count++;
    }

    float duration_sec = SAMPLE_SIZE / sample_rate_hz;
    return (peak_count / (duration_sec *
                          0.5f));  // Estimativa com aproximação de valor final
}