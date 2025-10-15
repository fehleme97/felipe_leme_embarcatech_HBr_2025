#include "inc/mpu6050.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "semphr.h"

#define MPU6050_ADDR 0x68

// Parâmetros gerais
#define CALIBRATION_SAMPLES 200
#define SAMPLE_INTERVAL_MS 100
#define BUFFER_SIZE 20

// Limiares de detecção
#define IMPACT_THRESHOLD 1.8f
#define INSTABILITY_THRESHOLD 0.6f
#define IMMOBILE_VARIATION 0.55f
#define IMMOBILE_CHECK_MS 2000
#define GYRO_IMMOBILE_THRESH 70.0f
#define FALL_WINDOW_MS 3000

// Estados
FALL_STATE fall_state = NORMAL;  // (atualizado via meq_get_state)

// Temporizações/flags originais
absolute_time_t fall_window_start;
absolute_time_t impact_time;
absolute_time_t alert_start;
bool alert_active = false;

static int16_t accel_offset[3] = {0};
static int16_t gyro_offset[3] = {0};

float acc_buffer[BUFFER_SIZE] = {0};
int buffer_index = 0;

static bool buffer_initialized = false;
static int buffer_filled = 0;

bool meq_update(MEQ_Runtime *rt, const MEQ_Config *cfg, float acc_mag,
                float gyro_mag, float variation) {
    switch (rt->state) {
        case NORMAL:
            if (variation > cfg->instability_threshold) {
                rt->fall_window_start = get_absolute_time();
                rt->state = PRE_FALL;
            }
            break;

        case PRE_FALL:
            if (acc_mag > cfg->impact_threshold_g) {
                rt->impact_time = get_absolute_time();
                rt->state = IMPACT;
            } else if (absolute_time_diff_us(rt->fall_window_start,
                                             get_absolute_time()) /
                           1000 >
                       cfg->fall_window_ms) {
                rt->state = NORMAL;
            }
            break;

        case IMPACT:
            if (absolute_time_diff_us(rt->impact_time, get_absolute_time()) /
                    1000 >
                cfg->immobile_check_ms) {
                if (variation < cfg->immobile_variation &&
                    gyro_mag < cfg->gyro_immobile_thresh) {
                    // Algoritmo confirmou queda
                    rt->state = POST_FALL;
                    return true;
                } else {
                    rt->state = NORMAL;
                }
            } else {
                // printf("[DEBUG] Aguardando estabilização...\n");
            }
            break;

        case POST_FALL:
            // o main decide o que fazer; depois chama meq_reset()
            break;
    }
    return false;
}

// Reseta o sensor MPU6050
static void mpu6050_reset() {
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(i2c0, MPU6050_ADDR, buf, 2, false);
    sleep_ms(100);
    buf[1] = 0x00;
    i2c_write_blocking(i2c0, MPU6050_ADDR, buf, 2, false);
    sleep_ms(10);

    buffer_initialized = false;
    buffer_filled = 0;
}

// Leitura do sensor MPU6050
static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[6];
    uint8_t val;

    val = 0x3B;  // Registrador inicial de leitura (aceleração)
    i2c_write_blocking(i2c0, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++)
        accel[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];

    val = 0x43;  // Registrador inicial de leitura (giroscópio)
    i2c_write_blocking(i2c0, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++)
        gyro[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];

    val = 0x41;  // Registrador inicial de leitura (temperatura)
    i2c_write_blocking(i2c0, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 2, false);
    *temp = (buffer[0] << 8) | buffer[1];
}

// Calibra o sensor MPU6050
static void calibrate_mpu() {
    int32_t acc_sum[3] = {0}, gyro_sum[3] = {0};
    int16_t acc[3], gyro[3], temp;

    printf("Iniciando calibragem...\n");
    for (int i = 0; i < CALIBRATION_SAMPLES;
         i++) {  // Lê os dados do MPU6050 várias vezes
        mpu6050_read_raw(acc, gyro, &temp);
        for (int j = 0; j < 3; j++) {
            acc_sum[j] += acc[j];
            gyro_sum[j] += gyro[j];
        }
        sleep_ms(10);
    }
    for (int j = 0; j < 3; j++) {
        accel_offset[j] = acc_sum[j] / CALIBRATION_SAMPLES;
        gyro_offset[j] = gyro_sum[j] / CALIBRATION_SAMPLES;
    }
    accel_offset[2] -= 16384;  // Corrigir gravidade
    printf("Offsets: AX=%d, AY=%d, AZ=%d\n", accel_offset[0], accel_offset[1],
           accel_offset[2]);
}

// Calcula a magnitude do vetor de aceleração, considera o offset e converte
// para g
float calculate_accel_magnitude(int16_t acc[3]) {
    float ax = (acc[0] - accel_offset[0]) / 16384.0f;
    float ay = (acc[1] - accel_offset[1]) / 16384.0f;
    float az = (acc[2] - accel_offset[2]) / 16384.0f;
    return sqrtf(ax * ax + ay * ay + az * az);
}

// Calcula a magnitude do vetor de giroscópio, considera o offset e converte
// para graus/s
float calculate_gyro_magnitude(int16_t gyro[3]) {
    float gx = (gyro[0] - gyro_offset[0]) / 131.0f;
    float gy = (gyro[1] - gyro_offset[1]) / 131.0f;
    float gz = (gyro[2] - gyro_offset[2]) / 131.0f;
    return sqrtf(gx * gx + gy * gy + gz * gz);
}

// Pré-preenche o buffer com a primeira amostra válida (evita salto 0→1g)
void add_to_buffer(float value) {
    if (!buffer_initialized) {
        for (int i = 0; i < BUFFER_SIZE; i++) {
            acc_buffer[i] = value;
        }
        buffer_index = 0;
        buffer_filled = BUFFER_SIZE;
        buffer_initialized = true;
        return;
    }

    acc_buffer[buffer_index] = value;
    buffer_index = (buffer_index + 1) % BUFFER_SIZE;
    if (buffer_filled < BUFFER_SIZE) buffer_filled++;
}

// Calcula a variação no buffer. Só calcula variação com buffer cheio; enquanto
// não encher, retorna 0
float buffer_variation() {
    if (buffer_filled < BUFFER_SIZE) return 0.0f;

    float min_val = acc_buffer[0], max_val = acc_buffer[0];
    for (int i = 1; i < BUFFER_SIZE; i++) {
        if (acc_buffer[i] < min_val) min_val = acc_buffer[i];
        if (acc_buffer[i] > max_val) max_val = acc_buffer[i];
    }
    return max_val - min_val;
}

void buffer_clear(void) {
    buffer_index = 0;
    buffer_filled = 0;
    buffer_initialized = false;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        acc_buffer[i] = 0.0f;
    }
}