#ifndef MAX30100_H
#define MAX30100_H

#include <stdbool.h>

#include "hardware/i2c.h"
#include "semphr.h"

#define MAX30102_ADDR 0x57

// Lista de endereços de registradores
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_ENABLE_1 0x02
#define REG_FIFO_WR_PTR 0x04
#define REG_FIFO_OVF_CNT 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C  // Corrente do LED RED
#define REG_LED2_PA 0x0D  // Corrente do LED IR
#define REG_PART_ID 0xFF  // Deve retornar 0x15

// Estrutura de saída
typedef struct {
    float bpm;
    float spo2;
} max30100_data_t;

#endif  // MAX30100_H
