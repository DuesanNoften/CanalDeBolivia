#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "canal.h"
#include <time.h>
#include <wifi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "wifi.h"

// Función para enviar datos del barco al servidor
int send_ship_data(const int *ship_data, size_t size) {
    // Inicializar el socket y conectarse al servidor
    if (initialize_socket() == -1) {
        return 0;  // Error al conectar
    }

    // Construir el mensaje a partir del array de datos
    char message[100];
    strcpy(message, "[");
    for (size_t i = 0; i < size; i++) {
        char buffer[10];
        sprintf(buffer, "%d", ship_data[i]);
        strcat(message, buffer);
        if (i < size - 1) strcat(message, ", ");
    }
    strcat(message, "]");

    // Enviar el mensaje al servidor
    if (send_message(message) == -1) {
        close(sock);  // Cerrar el socket en caso de error
        return 0;
    }

    // Cerrar el socket después del envío
    close(sock);
    printf("Socket cerrado tras enviar: %s\n", message);
    return 1;
}


void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships) {
    int method = config->flow_control_method;
    if (method == 0) {
        int w = config->W;  // Número de barcos a pasar en cada dirección
        int left_count, right_count, has_passed;

        printf("Comenzando el paso de barcos por el canal:\n");

        while (*left_ships || *right_ships) {
            // Pasar barcos del lado izquierdo
            left_count = 0;
            while (*left_ships && left_count < w) {
                Ship *ship = &(*left_ships)->ship;
                ship->thread.start_routine((void *)ship);

                // Crear los datos del barco para enviar
                if((*left_ships)->next == NULL) {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 0, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }
                else {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 1, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }

                Node *temp = *left_ships;
                *left_ships = (*left_ships)->next;
                free(temp);
                left_count++;
            }

            // Pasar barcos del lado derecho
            right_count = 0;
            while (*right_ships && right_count < w) {
                Ship *ship = &(*right_ships)->ship;
                ship->thread.start_routine((void *)ship);

                // Crear los datos del barco para enviar
                if((*right_ships)->next == NULL) {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }
                else {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 1};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }

                Node *temp = *right_ships;
                *right_ships = (*right_ships)->next;
                free(temp);
                right_count++;
            }
        }
    }

    else if (method == 1) {  // Enviar barcos por 20 segundos de cada lado
        printf("Comenzando el paso alternado de barcos por tiempo:\n");

        int left_turn = 1;  // Variable para alternar entre lados
        time_t start_time;

        while (*left_ships || *right_ships) {
            // Registrar el tiempo de inicio del turno actual
            start_time = time(NULL);

            if (left_turn) {
                printf("Turno del lado izquierdo.\n");
                while (*left_ships && time(NULL) - start_time < config->time_to_switch) {
                    Ship *ship = &(*left_ships)->ship;
                    ship->thread.start_routine((void *)ship);

                    // Crear los datos del barco para enviar
                    // Crear los datos del barco para enviar
                    if((*left_ships)->next == NULL) {
                        int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 0, 0};
                        send_ship_data(ship_data, 6);  // Enviar los datos
                    }
                    else {
                        int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 1, 0};
                        send_ship_data(ship_data, 6);  // Enviar los datos
                    }

                    Node *temp = *left_ships;
                    *left_ships = (*left_ships)->next;
                    free(temp);
                }
            } else {
                printf("Turno del lado derecho.\n");
                while (*right_ships && time(NULL) - config->time_to_switch) {
                    Ship *ship = &(*right_ships)->ship;
                    ship->thread.start_routine((void *)ship);

                    // Crear los datos del barco para enviar
                    if((*right_ships)->next == NULL) {
                        int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 0};
                        send_ship_data(ship_data, 6);  // Enviar los datos
                    }
                    else {
                        int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 1};
                        send_ship_data(ship_data, 6);  // Enviar los datos
                    }

                    Node *temp = *right_ships;
                    *right_ships = (*right_ships)->next;
                    free(temp);
                }
            }

            left_turn = !left_turn;  // Alternar el turno
        }
    }
    else if (method == 2) {
        // Inicializar la semilla para números aleatorios
        srand(time(NULL));

        while (*left_ships || *right_ships) {
            // Elegir aleatoriamente el próximo lado: 0 = derecho, 1 = izquierdo
            int random_side = rand() % 2;

            if (random_side == 1 && *left_ships) {  // Enviar desde el lado izquierdo
                Ship *ship = &(*left_ships)->ship;
                ship->thread.start_routine((void *)ship);

                // Crear los datos del barco para enviar
                if((*left_ships)->next == NULL) {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 0, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }
                else {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 0, 1, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }

                Node *temp = *left_ships;
                *left_ships = (*left_ships)->next;
                free(temp);
                printf("Barco enviado desde el lado izquierdo.\n");
            }
            else if (*right_ships) {  // Enviar desde el lado derecho
                Ship *ship = &(*right_ships)->ship;
                ship->thread.start_routine((void *)ship);

                // Crear los datos del barco para enviar
                if((*right_ships)->next == NULL) {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 0};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }
                else {
                    int ship_data[6] = {ship->time, ship->type, ship->direction, 1, 0, 1};
                    send_ship_data(ship_data, 6);  // Enviar los datos
                }

                Node *temp = *right_ships;
                *right_ships = (*right_ships)->next;
                free(temp);
                printf("Barco enviado desde el lado derecho.\n");
            }
        }
    }

    printf("Todos los barcos han pasado por el canal.\n");
}


// Funcion para reconocer si el string es un entero positivo
int is_positive_integer(const char *str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Funcion para setear la configuracion del canal desde la linea de comandos
void setCanalConfigFromFile(CanalConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value) {
            if (!is_positive_integer(value)) {
                printf("El valor para %s no es un entero positivo. Se establece en 0.\n", key);
                value = "0";
            }

            if (strcmp(key, "flow_control_method") == 0) {
                config->flow_control_method = atoi(value);
                if (config->flow_control_method < 0 || config->flow_control_method > 2) {
                    printf("El valor para flow_control_method no es válido. Se establece en 0.\n");
                    config->flow_control_method = 0;
                }
            } else if (strcmp(key, "canal_length") == 0) {
                config->canal_length = atoi(value);
            } else if (strcmp(key, "ship_speed") == 0) {
                config->ship_speed = atoi(value);
            } else if (strcmp(key, "lnum_ships") == 0) {
                config->lnum_ships = atoi(value);
            } else if (strcmp(key, "rnum_ships") == 0) {
                config->rnum_ships = atoi(value);
            } else if (strcmp(key, "time_to_switch") == 0) {
                config->time_to_switch = atoi(value);
            } else if (strcmp(key, "W") == 0) {
                config->W = atoi(value);
            } else if (strcmp(key, "scheduling_type") == 0) {  
                config->scheduling_type = atoi(value);
            }
        }
    }

    fclose(file);
}


