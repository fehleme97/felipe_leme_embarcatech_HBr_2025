#ifndef MPU6050_H
#define MPU6050_H

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "semphr.h"

typedef enum { NORMAL, PRE_FALL, IMPACT, POST_FALL } FALL_STATE;

typedef struct {
    float impact_threshold_g;     // IMPACT_THRESHOLD
    float instability_threshold;  // INSTABILITY_THRESHOLD (variação)
    float immobile_variation;     // IMMOBILE_VARIATION
    uint32_t immobile_check_ms;   // IMMOBILE_CHECK_MS
    float gyro_immobile_thresh;   // GYRO_IMMOBILE_THRESH
    uint32_t fall_window_ms;      // FALL_WINDOW_MS
} MEQ_Config;

// Runtime interno do módulo
typedef struct {
    FALL_STATE state;
    absolute_time_t fall_window_start;
    absolute_time_t impact_time;
} MEQ_Runtime;

// Inicializa o runtime
static inline void meq_init(MEQ_Runtime *rt) {
    rt->state = NORMAL;
    rt->fall_window_start = get_absolute_time();
    rt->impact_time = get_absolute_time();
}

// Atualiza a máquina de estados com métricas já calculadas no main.
// Retorna true SOMENTE quando a queda for confirmada pelo algoritmo
bool meq_update(MEQ_Runtime *rt, const MEQ_Config *cfg, float acc_mag,
                float gyro_mag, float variation);

// Acesso ao estado
static inline FALL_STATE meq_get_state(const MEQ_Runtime *rt) {
    return rt->state;
}

// Reset após tratar evento
static inline void meq_reset(MEQ_Runtime *rt) { rt->state = NORMAL; }

#endif
