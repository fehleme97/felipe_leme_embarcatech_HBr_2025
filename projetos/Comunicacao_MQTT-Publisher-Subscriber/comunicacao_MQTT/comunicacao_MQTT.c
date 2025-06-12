#include <stdio.h>
#include <time.h>  // Adicionado para usar time()
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

#include "mqtt_comm.h"

/**
 * Função para aplicar cifra XOR (criptografia/decifração)
 */
void xor_encrypt(const uint8_t *input, uint8_t *output, size_t len, uint8_t key) {
    for (size_t i = 0; i < len; ++i) {
        output[i] = input[i] ^ key;
    }
}

void configure_static_ip(struct netif *netif) {
    ip4_addr_t ipaddr, netmask, gw;

    IP4_ADDR(&ipaddr, XXX, XXX, XX, XXX); //IPV4
    IP4_ADDR(&gw, XXX, XXX, XX, X); //Gateway
    IP4_ADDR(&netmask, XXX, XXX, XXX, X); //Submascara

    netif_set_addr(netif, &ipaddr, &netmask, &gw);
    printf("IP fixo configurado: %s\n", ip4addr_ntoa(&ipaddr));
}

void check_connection_status() {
    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("Status da conexão: %d\n", status);
}

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
    const char *password = ""; //senha da rede

    connect_to_wifi(ssid, password);

    struct netif *netif = netif_list;

    if (netif != NULL) {
        configure_static_ip(netif);
    } else {
        printf("Erro: interface de rede não encontrada!\n");
    }

    mqtt_setup("clienteX", "XXX.XXX.XX.XX", "aluno", "senha123");

    while (true) {
        cyw43_arch_poll();
        check_connection_status();

        // Mensagem JSON com timestamp
        char buffer[64];
        time_t ts = time(NULL);
        snprintf(buffer, sizeof(buffer), "{\"valor\":26.5,\"ts\":%lu}", ts);

        printf("Mensagem JSON: %s\n", buffer);

        // Criptografa a mensagem JSON
        uint8_t criptografada[64];  // Buffer para mensagem criptografada
        xor_encrypt((uint8_t *)buffer, criptografada, strlen(buffer), 42);

        // Publica a mensagem criptografada
        mqtt_comm_publish("escola/sala1/temperatura", criptografada, strlen(buffer));

        sleep_ms(3000);
    }

    return 0;
}
