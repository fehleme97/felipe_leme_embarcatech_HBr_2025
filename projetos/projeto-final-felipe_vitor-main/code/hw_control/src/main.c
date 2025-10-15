#include <string.h>

#include "FreeRTOS.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

// Drivers
#include "drivers/max30100.c"  // usa i2c1
#include "drivers/mpu6050.c"   // usa i2c0
#include "drivers/mqtt.c"
#include "drivers/rgb_buzzer.c"

// Define os pinos
#define I2C0_SDA 0
#define I2C0_SCL 1
#define I2C1_SDA 2
#define I2C1_SCL 3
#define BTN_WARNING_PIN 5    // botão de confirmação de queda (ACK)
#define BTN_EMERGENCY_PIN 6  // botão de emergência

// Constantes
#define WATER_REMINDER_MS (1000 * 60 * 30)  // lembrete de água (30 min)
#define CONFIRM_WINDOW_SECONDS 10  // janela de confirmação de queda (10 seg)

// RTOS handles
static QueueHandle_t qHeart;            // dados vitais (MAX30102)
static SemaphoreHandle_t semUserAck;    // ACK do usuário (BTN_WARNING)
static SemaphoreHandle_t semEmergency;  // emergência (BTN_EMERGENCY)

// ISR: Botões
void gpio_callback(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (gpio == BTN_WARNING_PIN) {
        xSemaphoreGiveFromISR(semUserAck, &xHigherPriorityTaskWoken);
    } else if (gpio == BTN_EMERGENCY_PIN) {
        xSemaphoreGiveFromISR(semEmergency, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Task obter sinais vitais (MAX30102)
void task_heart(void *pv) {
    set_rgb_led(0, 1, 0);
    max30100_data_t d;

    const float sample_rate = 100.0f;

    while (1) {
        int samples_collected = 0;
        while (samples_collected < SAMPLE_SIZE) {
            uint32_t red, ir;
            if (max30102_read_sample(&red, &ir)) {
                red_buffer[samples_collected] = red;
                ir_buffer[samples_collected] = ir;
                samples_collected++;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        d.bpm = calculate_bpm(ir_buffer, sample_rate);
        d.spo2 = calculate_spo2(red_buffer, ir_buffer);
        printf("Heart Rate: %.1f BPM\tSpO2: %.1f%%\n", d.bpm, d.spo2);

        mqtt_pub_vitals((float)d.bpm, (float)d.spo2);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// Task detectar queda (MPU6050)
void task_fall(void *pv) {
    calibrate_mpu();

    int16_t acc[3], gyro[3], temp;
    float smooth_mag = 1.0f;

    MEQ_Runtime meq_rt;
    meq_init(&meq_rt);
    MEQ_Config meq_cfg = {.impact_threshold_g = IMPACT_THRESHOLD,
                          .instability_threshold = INSTABILITY_THRESHOLD,
                          .immobile_variation = IMMOBILE_VARIATION,
                          .immobile_check_ms = IMMOBILE_CHECK_MS,
                          .gyro_immobile_thresh = GYRO_IMMOBILE_THRESH,
                          .fall_window_ms = FALL_WINDOW_MS};

    while (1) {
        mpu6050_read_raw(acc, gyro, &temp);

        float acc_mag = calculate_accel_magnitude(acc);
        float gyro_mag = calculate_gyro_magnitude(gyro);
        smooth_mag = (smooth_mag * 4.0f + acc_mag) / 5.0f;
        add_to_buffer(smooth_mag);
        float variation = buffer_variation();

        bool fall_algo =
            meq_update(&meq_rt, &meq_cfg, acc_mag, gyro_mag, variation);

        if (fall_algo) {
            // 1) Janela de confirmação
            set_rgb_led(1, 0, 0);
            bool acknowledged = false;
            for (int t = CONFIRM_WINDOW_SECONDS; t > 0; --t) {
                printf("Possível queda, restam %ds\n", t);

                if (xSemaphoreTake(semUserAck, pdMS_TO_TICKS(1000)) == pdTRUE) {
                    acknowledged = true;
                    set_rgb_led(0, 1, 0);
                    break;
                }
            }

            if (acknowledged) {
                printf("[FALL] Usuário confirmou estar bem.\n");
                set_rgb_led(0, 1, 0);
                meq_reset(&meq_rt);
            } else {
                // 2) Alerta de queda até ACK
                mqtt_send_fall();
                printf("🚨 Queda confirmada!\n");

                while (1) {
                    set_rgb_led(1, 0, 0);
                    buzzer_beep(500);
                    vTaskDelay(pdMS_TO_TICKS(200));
                    set_rgb_led(0, 0, 0);
                    buzzer_beep(500);
                    vTaskDelay(pdMS_TO_TICKS(200));

                    if (xSemaphoreTake(semUserAck, 0) == pdTRUE) {
                        printf("[FALL] Usuário confirmou estar bem.\n");
                        set_rgb_led(0, 1, 0);
                        meq_reset(&meq_rt);
                        break;
                    }
                }
            }
            set_rgb_led(0, 1, 0);
            meq_reset(&meq_rt);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// Task emergência
void task_emergency(void *pv) {
    while (1) {
        if (xSemaphoreTake(semEmergency, portMAX_DELAY) == pdTRUE) {
            mqtt_send_emergency();
            printf("Botão emergência apertado!\n");
            for (int i = 0; i < 5; i++) {
                set_rgb_led(1, 0, 0);
                buzzer_beep(500);
                vTaskDelay(pdMS_TO_TICKS(200));
                set_rgb_led(0, 0, 0);
                buzzer_beep(500);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            set_rgb_led(0, 1, 0);
        }
    }
}

// task lembrete beber água
void task_water(void *pv) {
    TickType_t lastWake = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(WATER_REMINDER_MS);

    while (1) {
        vTaskDelayUntil(&lastWake, interval);

        printf("Hora de beber água!\n");
        set_rgb_led(0, 0, 1);
        buzzer_beep(3000);
        vTaskDelay(pdMS_TO_TICKS(5000));
        set_rgb_led(0, 1, 0);
    }
}

int main() {
    stdio_init_all();

    // Configura os I2C`s
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA);
    gpio_pull_up(I2C0_SCL);

    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA);
    gpio_pull_up(I2C1_SCL);

    // cria recursos RTOS
    qHeart = xQueueCreate(1, sizeof(max30100_data_t));
    semUserAck = xSemaphoreCreateBinary();
    semEmergency = xSemaphoreCreateBinary();

    if (!qHeart || !semUserAck || !semEmergency) {
        printf("Erro ao criar recursos FreeRTOS\n");
        while (1) tight_loop_contents();
    }

    // Drivers / periféricos
    mqtt_homecare_cfg_t mqcfg = {
        .wifi_ssid = NULL,
        .wifi_pass = NULL,
        .broker_ip = NULL,
        .broker_port = 1883,
        .client_id = NULL,
        .username = "seu_usuario",  // usuario broker
        .password = "sua_senha"     // senha broker
    };

    mqtt_init(&mqcfg);
    act_init();
    max30102_init();  // MAX30102 em i2c1
    mpu6050_reset();  // reset/config do MPU em i2c0

    // pull-up + IRQ dos botões
    gpio_init(BTN_WARNING_PIN);
    gpio_set_dir(BTN_WARNING_PIN, GPIO_IN);
    gpio_pull_up(BTN_WARNING_PIN);

    gpio_init(BTN_EMERGENCY_PIN);
    gpio_set_dir(BTN_EMERGENCY_PIN, GPIO_IN);
    gpio_pull_up(BTN_EMERGENCY_PIN);

    gpio_set_irq_enabled_with_callback(BTN_WARNING_PIN, GPIO_IRQ_EDGE_FALL,
                                       true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BTN_EMERGENCY_PIN, GPIO_IRQ_EDGE_FALL,
                                       true, &gpio_callback);

    xTaskCreate(task_heart, "Heart", 2048, NULL, 2, NULL);
    xTaskCreate(task_fall, "Fall", 2048, NULL, 3, NULL);
    xTaskCreate(task_emergency, "Emergency", 2048, NULL, 4, NULL);
    xTaskCreate(task_water, "Water", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {
        tight_loop_contents();
    }
    return 0;
}
