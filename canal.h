#ifndef CANAL_H
#define CANAL_H

#include "Scheduling/scheduling.h"
#include "CEthreads/CEthread.h"
#include <ctype.h>



// Estructura que representa la configuración del canal
typedef struct {
    int flow_control_method;  // Método de control de flujo (0: Equidad, 1: Letrero, 2: Tico)
    int canal_length;         // Longitud del canal
    int ship_speed;           // Velocidad del barco
    int lnum_ships;            // Cantidad de barcos izquierda
    int rnum_ships;            // Cantidad de barcos derecha
    int time_to_switch;       // Tiempo para cambiar el letrero (en caso de usar Letrero)
    int W;
    int scheduling_type;                    // Parámetro W (en caso de usar Equidad)
} CanalConfig;

// Prototipo de la función para iniciar el canal
void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships);

void setCanalConfigFromFile(CanalConfig *config, const char *filename);

// Declaración de la función para reconocer si el string es un entero positivo
int is_positive_integer(const char *str);

typedef struct {
    int duration;    // Duration of the timer in seconds
    int stopped;     // Status of the timer: 1 if stopped, 0 otherwise
} TimerData;

void *timer_thread(void *arg);
#endif // CANAL_H
