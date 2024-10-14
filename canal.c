#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "canal.h"
#include <time.h>
#include <SDL2/SDL.h> 

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Limpiar SDL
void cleanup_SDL() {
    SDL_Quit();
}

void render_ships(SDL_Renderer* renderer, Node* head){
    Node* current =head;
    while (current != NULL){
        drawShip(renderer, &current->ship);
        current = current->next;
        SDL_RenderPresent(renderer);
    }
    
}
/*
// Renderizar barcos en el canal
void render_ships(SDL_Renderer *renderer, Node *left_ships, Node *right_ships) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Fondo negro
    SDL_RenderClear(renderer);  // Limpiar la pantalla

    // Dibujar barcos del lado izquierdo
    Node *temp = left_ships;
    int y_pos = 275;
    while (temp != NULL) {
        SDL_Rect ship_rect = {50, y_pos, 50, 30};  // Barco de ejemplo
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde para barcos de la izquierda
        SDL_RenderFillRect(renderer, &ship_rect);
        temp = temp->next;
        y_pos += 35;  // Ajustar posición
    }

    // Dibujar barcos del lado derecho
    temp = right_ships;
    y_pos = 275;
    while (temp != NULL) {
        SDL_Rect ship_rect = {WINDOW_WIDTH - 100, y_pos, 50, 30};  // Barco de ejemplo
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Azul para barcos de la derecha
        SDL_RenderFillRect(renderer, &ship_rect);
        temp = temp->next;
        y_pos += 35;
    }

    SDL_RenderPresent(renderer);  // Actualizar la pantalla
}*/
// Move the ship (on a thread)

void drawShip(SDL_Renderer* renderer, Ship* ship);

void drawShip(SDL_Renderer* renderer, Ship* ship) {
    // Change the color between ship types
    if (ship->type == 1) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red for the normal ones
    } else if (ship->type == 2) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);  // Green for fishing ones
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);  // Blue for patrol ones
    }
    //Draw the ship rectangle and update window
    SDL_Rect shipRect = { ship->x, ship->y, 50, 20 };
    SDL_RenderFillRect(renderer, &shipRect);
    SDL_RenderPresent(renderer);
}

void start_canal(CanalConfig *config, Node **left_ships, Node **right_ships) {
    //Logica paso de barcos
    int method = config->flow_control_method;
    if (method == 0) {
        int w = config->W;  // Número de barcos a pasar en cada dirección
        int left_count, right_count;

        printf("Comenzando el paso de barcos por el canal:\n");

        while (*left_ships || *right_ships) {
            // Pasar barcos del lado izquierdo
            left_count = 0;
            while (*left_ships && left_count < w) {
                Ship *ship = &(*left_ships)->ship;
                
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
                    
                    Node *temp = *left_ships;
                    *left_ships = (*left_ships)->next;
                    free(temp);
                }
            } else {
                printf("Turno del lado derecho.\n");
                while (*right_ships && time(NULL) - config->time_to_switch) {
                    Ship *ship = &(*right_ships)->ship;
                    ship->thread.start_routine((void *)ship);
                    
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
                
                Node *temp = *left_ships;
                *left_ships = (*left_ships)->next;
                free(temp);
                printf("Barco enviado desde el lado izquierdo.\n");
            }
            else if (*right_ships) {  // Enviar desde el lado derecho
                Ship *ship = &(*right_ships)->ship;
                ship->thread.start_routine((void *)ship);
                
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
                printf("Valor de canal_length = %d\n", config->canal_length);
            } else if (strcmp(key, "ship_speed") == 0) {
                config->ship_speed = atoi(value);
                printf("Valor de ship_speed = %d\n", config->ship_speed);
            } else if (strcmp(key, "lnum_ships") == 0) {
                config->lnum_ships = atoi(value);
                printf("Valor de lnum_ships = %d\n", config->lnum_ships);
            } else if (strcmp(key, "rnum_ships") == 0) {
                config->rnum_ships = atoi(value);
                printf("Valor de rnum_ships = %d\n", config->rnum_ships);
            } else if (strcmp(key, "time_to_switch") == 0) {
                config->time_to_switch = atoi(value);
                printf("Valor de time_to_switch = %d\n", config->time_to_switch);
            } else if (strcmp(key, "W") == 0) {
                config->W = atoi(value);
                printf("Valor de W = %d\n", config->W);
            } else if (strcmp(key, "scheduling_type") == 0) {  
                config->scheduling_type = atoi(value);
                printf("Valor de scheduling_type = %d\n", config->scheduling_type);
            }
        }
    }

    fclose(file);
}


