#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include "lwip/apps/mqtt.h"

void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass);

#endif
