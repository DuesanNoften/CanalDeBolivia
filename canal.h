#ifndef CANAL_H
#define CANAL_H

#include "Scheduling/scheduling.h"
#include "CEthreads/CEthread.h"

// Estructura que representa la configuración del canal
typedef struct {
    int flow_control_method;  // Método de control de flujo (0: Equidad, 1: Letrero, 2: Tico)
    int canal_length;         // Longitud del canal
    int ship_speed;           // Velocidad del barco
    int num_ships;            // Cantidad de barcos en las colas
    int time_to_switch;       // Tiempo para cambiar el letrero (en caso de usar Letrero)
    int W;                    // Parámetro W (en caso de usar Equidad)
} CanalConfig;

// Prototipo de la función para iniciar el canal
void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships);

// Declaración de la función para reconocer si el string es un entero positivo
int is_positive_integer(const char str);

// Declaración de la función para establecer la configuración del canal desde un archivo
void setCanalConfigFromFile(CanalConfigconfig, const char *filename);
#endif // CANAL_H
