#include "lwip/apps/mqtt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "lwip/ip_addr.h"
#include "mqtt.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "semphr.h"
#include "task.h"

// Defini Wifi + MQTT
#ifndef WIFI_SSID_DEFAULT
#define WIFI_SSID_DEFAULT "seu_ssid"
#endif
#ifndef WIFI_PASS_DEFAULT
#define WIFI_PASS_DEFAULT "sua_senha"
#endif
#ifndef MQTT_BROKER_IP_DEFAULT
#define MQTT_BROKER_IP_DEFAULT "mqtt_ip_broker"
#endif
#ifndef MQTT_BROKER_PORT_DEFAULT
#define MQTT_BROKER_PORT_DEFAULT 1883
#endif
#ifndef MQTT_CLIENT_ID_DEFAULT
#define MQTT_CLIENT_ID_DEFAULT "mqtt_client_id"
#endif

#define VITALS_HZ 1  // taxa de “saída” (amostras/s) para o buffer
#define VITALS_WINDOW_SEC (5 * 60)                       // 5 minutos
#define VITALS_CAPACITY (VITALS_HZ * VITALS_WINDOW_SEC)  // 300 amostras

typedef struct {
    uint32_t ts_ms;
    float bpm;
    float spo2;
} vitals_sample_t;

typedef struct {
    vitals_sample_t buf[VITALS_CAPACITY];
    size_t head;
    size_t count;
    SemaphoreHandle_t lock;
} vitals_ring_t;

static struct {
    const char *ssid;
    const char *pass;
    const char *broker_ip;
    uint16_t broker_port;
    const char *client_id;

    mqtt_client_t *client;
    SemaphoreHandle_t sem_connected;

    vitals_ring_t vitals;
} g_mq = {0};

// Declaração dos tópicos
#define TOPIC_VITALS_POINT_FMT "homecare/bracelet/%s/vitals"
#define TOPIC_EVENT_EMERGENCY_FMT "homecare/bracelet/%s/event/emergency"
#define TOPIC_EVENT_FALL_FMT "homecare/bracelet/%s/event/fall"
#define TOPIC_CMD_ALARM_ACK_FMT "homecare/bracelet/%s/cmd/alarm_ack"
#define TOPIC_CMD_SILENCE_FMT "homecare/bracelet/%s/cmd/silence"
#define TOPIC_CMD_PING_FMT "homecare/bracelet/%s/cmd/ping"

static inline uint32_t now_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

static void vr_init(vitals_ring_t *r) {
    memset(r, 0, sizeof(*r));
    r->lock = xSemaphoreCreateMutex();
}

static void vr_push(vitals_ring_t *r, float bpm, float spo2) {
    xSemaphoreTake(r->lock, portMAX_DELAY);
    r->buf[r->head].ts_ms = now_ms();
    r->buf[r->head].bpm = bpm;
    r->buf[r->head].spo2 = spo2;
    r->head = (r->head + 1) % VITALS_CAPACITY;
    if (r->count < VITALS_CAPACITY) r->count++;
    xSemaphoreGive(r->lock);
}

// Callbacks MQTT
// Chegou publicação em um tópico assinado
static void inpub_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("[MQTT] in topic='%s' len=%lu\n", topic, (unsigned long)tot_len);
}

// Processa payloads (JSON/Texto)
static void indata_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("[MQTT] payload: %.*s\n", (int)len, (const char *)data);
}

static void conn_cb(mqtt_client_t *c, void *arg,
                    mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("[MQTT] connected\n");
        xSemaphoreGive(g_mq.sem_connected);

        // Assina comandos básicos do ESP32 (para expansão futura)
        char t1[128], t2[128], t3[128];
        snprintf(t1, sizeof t1, TOPIC_CMD_ALARM_ACK_FMT, g_mq.client_id);
        snprintf(t2, sizeof t2, TOPIC_CMD_SILENCE_FMT, g_mq.client_id);
        snprintf(t3, sizeof t3, TOPIC_CMD_PING_FMT, g_mq.client_id);
        mqtt_subscribe(c, t1, 1, NULL, NULL);
        mqtt_subscribe(c, t2, 1, NULL, NULL);
        mqtt_subscribe(c, t3, 1, NULL, NULL);

        /* Birth: ficou online (retain) */
        char st_topic[128] = {0};
        snprintf(st_topic, sizeof st_topic, "homecare/bracelet/%s/status",
                 g_mq.client_id);
        mqtt_publish(g_mq.client, st_topic, "online", 6, 1, 1, NULL, NULL);

    } else {
        printf("[MQTT] disconnected (status=%d)\n", status);
    }
}

static err_t pub_str(const char *topic, const char *payload) {
    if (!g_mq.client) return ERR_VAL;
    const u8_t qos = 1;
    const u8_t retain = 0;
    err_t rc = mqtt_publish(g_mq.client, topic, payload, (u16_t)strlen(payload),
                            qos, retain, NULL, NULL);
    if (rc != ERR_OK) {
        printf("[MQTT] publish FAIL rc=%d topic=%s len=%u\n", (int)rc, topic,
               (unsigned)strlen(payload));
    } else {
        printf("[MQTT] publish OK topic=%s\n", topic);
    }
    return rc;
}

// Tasks Wi-Fi e MQTT
static void task_wifi(void *pv) {
    if (cyw43_arch_init()) {
        printf("[WiFi] init fail\n");
        vTaskDelete(NULL);
        return;
    }
    cyw43_arch_enable_sta_mode();
    printf("[WiFi] connecting to %s ...\n", g_mq.ssid);
    int r = cyw43_arch_wifi_connect_timeout_ms(g_mq.ssid, g_mq.pass,
                                               CYW43_AUTH_WPA2_AES_PSK, 15000);
    printf("[WiFi] connect result=%d\n", r);
    for (;;) vTaskDelay(pdMS_TO_TICKS(5000));
}

static void task_mqtt(void *pv) {
    ip_addr_t ip;
    ipaddr_aton(g_mq.broker_ip, &ip);
    g_mq.client = mqtt_client_new();
    mqtt_set_inpub_callback(g_mq.client, inpub_cb, indata_cb, NULL);

    // Client info (apenas uma vez)
    struct mqtt_connect_client_info_t ci = {0};
    ci.client_id = g_mq.client_id;
    ci.keep_alive = 30;

    // LWT: se o Pico cair sem DISCONNECT, publica 'offline' com retain
    static char will_topic[128];
    snprintf(will_topic, sizeof will_topic, "homecare/bracelet/%s/status",
             g_mq.client_id);
    ci.will_topic = will_topic;
    ci.will_msg = "offline";
    ci.will_qos = 1;
    ci.will_retain = 1;

    for (;;) {
        printf("[MQTT] connecting to %s:%d\n", g_mq.broker_ip,
               (int)g_mq.broker_port);
        err_t e = mqtt_client_connect(g_mq.client, &ip, g_mq.broker_port,
                                      conn_cb, NULL, &ci);
        if (e != ERR_OK) {
            vTaskDelay(pdMS_TO_TICKS(3000));
            continue;
        }
        xSemaphoreTake(g_mq.sem_connected, pdMS_TO_TICKS(15000));
        while (mqtt_client_is_connected(g_mq.client))
            vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

// Inicializa o Wi-Fi e MQTT
void mqtt_homecare_init(const mqtt_homecare_cfg_t *cfg) {
    g_mq.ssid = cfg && cfg->wifi_ssid ? cfg->wifi_ssid : WIFI_SSID_DEFAULT;
    g_mq.pass = cfg && cfg->wifi_pass ? cfg->wifi_pass : WIFI_PASS_DEFAULT;
    g_mq.broker_ip =
        cfg && cfg->broker_ip ? cfg->broker_ip : MQTT_BROKER_IP_DEFAULT;
    g_mq.broker_port = cfg ? cfg->broker_port : MQTT_BROKER_PORT_DEFAULT;
    g_mq.client_id =
        cfg && cfg->client_id ? cfg->client_id : MQTT_CLIENT_ID_DEFAULT;

    g_mq.sem_connected = xSemaphoreCreateBinary();
    vr_init(&g_mq.vitals);  // opcional, não decide persistência

    xTaskCreate(task_wifi, "wifi", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(task_mqtt, "mqtt", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
}

// Publica uma amostra de sinais vitais
void mqtt_homecare_pub_vitals(float bpm, float spo2) {
    if (!g_mq.client || !mqtt_client_is_connected(g_mq.client)) return;

    char topic[128], payload[160];
    snprintf(topic, sizeof topic, TOPIC_VITALS_POINT_FMT, g_mq.client_id);
    snprintf(payload, sizeof payload,
             "{\"ts_ms\":%u,\"bpm\":%.2f,\"spo2\":%.2f}", (unsigned)now_ms(),
             bpm, spo2);

    pub_str(topic, payload);
}

// Só guarda contexto local
void mqtt_homecare_push_vitals(float bpm, float spo2) {
    vr_push(&g_mq.vitals, bpm, spo2);
}

// Envia a mensagem de emergência
void mqtt_homecare_send_emergency(void) {
    if (!g_mq.client || !mqtt_client_is_connected(g_mq.client)) return;

    char topic[128], ev[128];
    snprintf(topic, sizeof topic, TOPIC_EVENT_EMERGENCY_FMT, g_mq.client_id);
    snprintf(ev, sizeof ev, "{\"ts_ms\":%u,\"src\":\"button\"}",
             (unsigned)now_ms());
    pub_str(topic, ev);
}

// Envia a mensagem de queda
void mqtt_homecare_send_fall(void) {
    if (!g_mq.client || !mqtt_client_is_connected(g_mq.client)) return;

    char topic[128], ev[128];
    snprintf(topic, sizeof topic, TOPIC_EVENT_FALL_FMT, g_mq.client_id);
    snprintf(ev, sizeof ev, "{\"ts_ms\":%u,\"src\":\"fall\"}",
             (unsigned)now_ms());
    pub_str(topic, ev);
}
