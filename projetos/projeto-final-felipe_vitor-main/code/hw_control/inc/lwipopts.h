#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS 1

#define LWIP_SOCKET 0
#define LWIP_NETCONN 0
#define LWIP_RAW 1

#define SYS_LIGHTWEIGHT_PROT 1

#define LWIP_IPV4 1
#define LWIP_IPV6 0

#define LWIP_ARP 1
#define LWIP_DHCP 1
#define LWIP_DNS 1
#define LWIP_IGMP 0

#define LWIP_NETIF_HOSTNAME 1

#define LWIP_TCP 1

#define LWIP_ALTCP 0
#define LWIP_ALTCP_TLS 0

#define MEM_ALIGNMENT 4

#define MEM_SIZE (24 * 1024)

#define PBUF_POOL_SIZE 32
#define PBUF_POOL_BUFSIZE 512

#define MEMP_NUM_PBUF 32
#define MEMP_NUM_TCP_PCB 8
#define MEMP_NUM_TCP_PCB_LISTEN 2
#define MEMP_NUM_TCP_SEG 64

/* Janela/tamanho de envio TCP — bom compromisso para Wi-Fi local */
#define TCP_MSS 1460
#define TCP_SND_BUF (8 * TCP_MSS)
#define TCP_WND (8 * TCP_MSS)
#define TCP_SND_QUEUELEN (4 * TCP_SND_BUF / TCP_MSS)

#define MQTT_REQ_MAX_IN_FLIGHT 5

#define LWIP_PROVIDE_ERRNO 1
#define LWIP_STATS 0
#define LWIP_DEBUG 0

#ifdef LWIP_NUM_SYS_TIMEOUT_INTERNAL
#define MEMP_NUM_SYS_TIMEOUT (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 10)
#else
#define MEMP_NUM_SYS_TIMEOUT 24
#endif

#endif
