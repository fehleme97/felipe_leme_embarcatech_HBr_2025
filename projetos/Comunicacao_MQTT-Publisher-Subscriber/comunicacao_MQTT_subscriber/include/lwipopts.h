#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS                          1
#define LWIP_SOCKET                     0
#define LWIP_NETCONN                    0
#define LWIP_RAW                        1

#define LWIP_NETIF_HOSTNAME             1   

#define MEMP_NUM_SYS_TIMEOUT   (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 1)
#define MQTT_REQ_MAX_IN_FLIGHT  (5)

#define LWIP_IPV4                       1
#define LWIP_DNS                        1
#define LWIP_IGMP                       1

#define LWIP_ALTCP                      0
#define LWIP_ALTCP_TLS                  0

#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        1600

#define LWIP_STATS                      0
#define LWIP_PROVIDE_ERRNO              1

#endif /* __LWIPOPTS_H__ */
