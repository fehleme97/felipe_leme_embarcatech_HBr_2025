#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/apps/mqtt.h"

#define TOPIC "escola/sala1/temperatura"
#define MQTT_BROKER "XXX.XXX.XX.XX" // IP DO BROKER HOSPEDANDO
#define XOR_KEY 42

static mqtt_client_t *client;

/**
 * Função para descriptografar via XOR
 */
void xor_decrypt(const uint8_t *input, uint8_t *output, size_t len, uint8_t key) {
    for (size_t i = 0; i < len; ++i) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0';  // Garante terminação nula
}

/**
 * Callback de chegada de mensagens
 */
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("Mensagem recebida no tópico: %s (tamanho: %lu)\n", topic, (unsigned long)tot_len);
}

/**
 * Callback de chegada de dados
 */
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    uint8_t decrypted[128];  // buffer seguro para descriptografar
    xor_decrypt(data, decrypted, len, XOR_KEY);

    char output_str[256];
    sprintf(output_str, "Dados descriptografados: %s", decrypted);
    printf("%s\n", output_str);
}

/**
 * Callback de inscrição no tópico
 */
static void mqtt_sub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Inscrição bem-sucedida no tópico.\n");
    } else {
        printf("Erro ao inscrever no tópico. Código: %d\n", result);
    }
}

/**
 * Função para realizar a inscrição no tópico
 */
static void mqtt_do_subscribe() {
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);
    mqtt_subscribe(client, TOPIC, 0, mqtt_sub_request_cb, NULL);
}

/**
 * Callback de conexão
 */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Conectado ao broker MQTT com sucesso!\n");
        mqtt_do_subscribe();
    } else {
        printf("Falha ao conectar ao broker, código: %d\n", status);
    }
}

/**
 * Configura IP fixo
 */
void configure_static_ip(struct netif *netif) {
    ip4_addr_t ipaddr, netmask, gw;

    IP4_ADDR(&ipaddr, XXX, XXX, XX, XXX);  // IP diferente do Publisher
    IP4_ADDR(&gw, XXX, XXX, XX, X; // Gateway
    IP4_ADDR(&netmask, XXX, XXX, XXX, X); //Submascara

    netif_set_addr(netif, &ipaddr, &netmask, &gw);
    printf("IP fixo configurado: %s\n", ip4addr_ntoa(&ipaddr));
}

/**
 * Checa status da conexão Wi-Fi
 */
void check_connection_status() {
    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("Status da conexão: %d\n", status);
}

/**
 * Conecta ao Wi-Fi
 */
void connect_to_wifi(const char *ssid, const char *password) {
    if (cyw43_arch_init()) {
        printf("Erro ao iniciar Wi-Fi\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi: %s\n", ssid);
    int result = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000);
    if (result) {
        printf("Erro ao conectar. Código: %d\n", result);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    const char *ssid = ""; // Login da rede
    const char *password = ""; // Senha da rede

    connect_to_wifi(ssid, password);

    struct netif *netif = netif_list;

    if (netif != NULL) {
        configure_static_ip(netif);
    } else {
        printf("Erro: interface de rede não encontrada!\n");
    }

    ip_addr_t broker_addr;
    if (!ip4addr_aton(MQTT_BROKER, &broker_addr)) {
        printf("Erro ao converter IP do broker.\n");
        return -1;
    }

    client = mqtt_client_new();
    if (client == NULL) {
        printf("Falha ao criar o cliente MQTT\n");
        return -1;
    }

    struct mqtt_connect_client_info_t ci = {
        .client_id = "subscriberX",
        .client_user = "aluno",
        .client_pass = "senha123"
    };

    mqtt_client_connect(client, &broker_addr, MQTT_PORT, mqtt_connection_cb, NULL, &ci);

    while (true) {
        cyw43_arch_poll();
        check_connection_status();
        sleep_ms(3000);
    }

    return 0;
}
