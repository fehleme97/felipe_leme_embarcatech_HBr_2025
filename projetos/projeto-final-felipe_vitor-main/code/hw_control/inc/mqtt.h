#ifndef MQTT_HOMECARE_H
#define MQTT_HOMECARE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *wifi_ssid;  ///< SSID da rede cliente (STA)
    const char *wifi_pass;  ///< Senha da rede
    const char *broker_ip;  ///< IP do broker MQTT (ESP32)
    uint16_t broker_port;   ///< Porta do broker (1883)
    const char *client_id;  ///< ID do dispositivo (ex.: "pico-homecare-01")
} mqtt_homecare_cfg_t;

/** Inicializa Wi-Fi + MQTT e cria as tasks internas. Não bloqueia. */
void mqtt_homecare_init(const mqtt_homecare_cfg_t *cfg);

/** Publica evento de EMERGÊNCIA (acionamento do botão). */
void mqtt_homecare_send_emergency(void);

/** Publica evento de QUEDA (quando confirmada pelo seu algoritmo). */
void mqtt_homecare_send_fall(void);

/**
 * Publica UMA amostra de vitais (BPM/SpO₂) imediatamente.
 * Chame isso quando você finalizar o cálculo (ex.: ao fechar um bloco de
 * amostras).
 */
void mqtt_homecare_pub_vitals(float bpm, float spo2);

/**
 * (Opcional) Insere vitais no buffer circular (5 min) para uso futuro (ex.:
 * lote). Não decide persistência local — só mantém contexto em RAM.
 */
void mqtt_homecare_push_vitals(float bpm, float spo2);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_HOMECARE_H */
