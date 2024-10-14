#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include "CEthreads/CEthread.h"
#include "Scheduling/scheduling.h"

#define SERVER_IP "192.168.0.16"  // Cambia esto por la IP de tu Raspberry Pi Pico W
#define SERVER_PORT 8080           // Puerto en el que está escuchando el servidor

extern int sock;

void handle_exit(int sig);
int initialize_socket();
int send_message(const char *message);



#endif // WIFI_H