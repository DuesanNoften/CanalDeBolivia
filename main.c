#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Scheduling/scheduling.h"
#include "CEthreads/CEthread.h"
#include "canal.h"
#include <SDL2/SDL.h> 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Prototipo de la función de procesamiento del barco
int process_ship_thread(void *arg) {
    Ship *ship = (Ship *)arg;

    printf("El hilo barcoID: %d comenzo\n", ship->id);
    //CEmutex_lock(&ship->mutex);
    while (1) {

        // Move the ship through the canal (one at a time)
        if (ship->side == 0) {  // Left to right
            while (ship->x < WINDOW_WIDTH / 2) {
                ship->x += ship->type;
                usleep(10000);  // Control the speed
            }
        } else {  // Right to left
            while (ship->x > WINDOW_WIDTH / 2) {
                ship->x -= ship->type;
                usleep(10000);  // Control the speed
            }
        }
        printf("%d type ship is crossing in direction %d.\n", ship->type, ship->side);

        // Continue moving the ship out of the canal
        if (ship->side == 0) {  // Left to right
            while (ship->x < WINDOW_WIDTH-30) {
                ship->x += ship->type;
                usleep(10000);  // Control the speed
            }
        } else {  // Right to left
            while (ship->x > 0) {  // Move out of the screen on the left
                ship->x -= ship->type;
                usleep(10000);  // Control the speed
            }
        }

        break;  // The ship has finished crossing the canal
    }
    sleep(ship->time); 
    printf("El hilo barcoID: %d termino.\n", ship->id);
    //CEmutex_unlock(&ship->mutex);

    return 0;
}

//SDL functions
int initSDL(SDL_Window **window, SDL_Renderer **renderer);
void closeSDL(SDL_Window *window, SDL_Renderer *renderer);

// Función para crear un barco y asignarle valores
Ship create_ship(int id, int priority, int time, int side, int type) {
    Ship ship;
    ship.id = id;
    ship.priority = priority;
    ship.time = time;
    ship.side = side;
    ship.type = type;
    ship.remaining_time = time; // Inicialmente, el tiempo restante es igual al tiempo de procesamiento
    ship.real_time_max = time + 5; // Asumir un tiempo máximo para pasar por el canal
    if(side == 0){
        ship.x = 30;
    }else{
        ship.x = 720;
    }
    //ship.y = 275 + rand()%30;
    ship.y = id * 25;

    // Inicializar el mutex del barco
    if (CEmutex_init(&ship.mutex) != 0) {
        printf("Error al inicializar el mutex para el barco con ID: %d\n", ship.id);
        exit(EXIT_FAILURE);
    }

    // Configurar el hilo del barco
    ship.thread.start_routine = process_ship_thread;
    ship.thread.arg = &ship;  // Pasar el puntero del barco al hilo
    ship.thread.stack_size = 1024 * 1024; // Asignar tamaño de pila (1 MB)

    // Crear el hilo para el barco
    if (CEthread_create(&ship.thread) == -1) {
        printf("Error al crear el hilo para el barco con ID: %d\n", ship.id);
        exit(EXIT_FAILURE);
    }

    printf("BarcoID: %d creado con hilo.\n", ship.id);

    return ship;
}

Ship create_random_ship(int id);

Ship create_random_ship(int id) {
    int priority = rand() % 5+1;
    int time = rand() % 5+1;
    int side = rand()%2;
    int type = rand() % 3 +1;
    Ship nuevoBarco = create_ship(id, priority, time, side, type);

    printf("Barco tipo %d generado con ID=%d, prioridad=%d, dirección=%d, tiempo=%d.\n", 
           type, id, priority, side, time);

    return nuevoBarco;
}

void insert_ship(Node **head, Ship ship, SDL_Renderer *renderer) {
    drawShip(renderer, &ship);
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->ship = ship;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}



void schedule_ships(CanalConfig *canal_config, Node **left_ships, Node **right_ships) {
    int scheduling_type = canal_config->scheduling_type;

    switch (scheduling_type) {
        case 1:  // Planificación por prioridad
            printf("\nProbando Prioridad:\n");
            priority_scheduling(left_ships);  
            priority_scheduling(right_ships);
            break;

        case 2:  // Planificación Round-Robin
            printf("\nProbando Round Robin:\n");
            round_robin(left_ships, 2);  
            round_robin(right_ships, 2);  
            break;

        case 3:  // Planificación SJF (Shortest Job First)
            printf("\nProbando SJF (Shortest Job First):\n");
            sjf_scheduling(left_ships);
            sjf_scheduling(right_ships);
            break;

        case 4:  // Planificación FCFS (First Come First Serve)
            printf("\nProbando FCFS (First Come First Serve):\n");
            fcfs_scheduling(left_ships);
            fcfs_scheduling(right_ships);
            break;

        case 5:  // Planificación para tiempo real
            printf("\nProbando Planificación en Tiempo Real:\n");
            real_time_scheduling(left_ships);
            real_time_scheduling(right_ships);
            break;

        default:  // Caso por defecto si no se reconoce el tipo de planificación
            printf("Error: Tipo de planificación no reconocido.\n");
            break;
    }
}


int main() {

    srand(clock());
    // Configuración del canal
    CanalConfig canal_config;
    setCanalConfigFromFile(&canal_config, "../CanalConfig.txt");

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL
    if (initSDL(&window, &renderer) != 0) {
        printf("Error initializing SDL\n");
        return -1;
    }

    // Crear listas de barcos
    Node *left_ships = NULL;
    Node *right_ships = NULL;

    // Crear algunos barcos y añadirlos a las listas
    int ships_created = 0;
    for (int i = 0; i < canal_config.lnum_ships; i++) {
        //Ship ship = create_ship(i + 1, 1, rand() % 5 + 1); // ID, Prioridad, Tiempo (1-5)
        Ship ship = create_ship(i + 1, rand() % 5 + 1, rand() % 5 + 1, 0, rand()%3+1); 
        insert_ship(&left_ships, ship, renderer);
        ships_created++;
    }

    for (int i = 0; i < canal_config.rnum_ships; i++) {
        //Ship ship = create_ship(i + 6, 1, rand() % 5 + 1); // ID, Prioridad, Tiempo (1-5)
        Ship ship = create_ship(ships_created+1, rand() % 5 + 1, rand() % 5 + 1, 1, rand()%3+1); 
        insert_ship(&right_ships, ship, renderer);
        ships_created++;
    }

    //Calendarizacion
    printf("Lista de barcos en la izquierda inicial:\n");
    Node *temp = left_ships;
    while (temp != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d, tipo=%d\n",
               temp->ship.id, temp->ship.priority, temp->ship.time, temp->ship.real_time_max, temp->ship.type);
        temp = temp->next;
    }

    printf("Lista de barcos en la derecha inicial:\n");
    Node *temp2 = right_ships;
    while (temp2 != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d, tipo=%d\n",
               temp2->ship.id, temp2->ship.priority, temp2->ship.time, temp2->ship.real_time_max, temp2->ship.type);
        temp2 = temp2->next;
    }

    schedule_ships(&canal_config,&left_ships, &right_ships);

    printf("\nLista de barcos en la izquierda despues de aplicar el algoritmo:\n");
    temp = left_ships;
    while (temp != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d\n",
               temp->ship.id, temp->ship.priority, temp->ship.time, temp->ship.real_time_max);
        temp = temp->next;
    }

    printf("\nLista de barcos en la derecha despues de aplicar el algoritmo:\n");
    temp = right_ships;
    while (temp != NULL) {
        printf("Barco ID: %d, Prioridad: %d, Tiempo: %d, Tiempo real máximo: %d\n", 
               temp->ship.id, temp->ship.priority, temp->ship.time, temp->ship.real_time_max);
        temp = temp->next;
    }

    

    int running = 1;
    SDL_Event e;
    const int frame_delay = 16;
    Uint32 frame_start;
    int frame_time;
    Ship new_ship;

    while (running){
        frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                running = 0;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym){
                    case SDLK_SPACE:
                        new_ship = create_random_ship(ships_created+1);
                        
                        if (new_ship.side == 0){
                            insert_ship(&left_ships, new_ship, renderer);
                        }else{
                            insert_ship(&right_ships, new_ship, renderer);
                        }
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;
                        break;
                    // Ships left to right
                    case SDLK_q:  // Patrol left
                        new_ship = create_ship(ships_created+1, 5, rand()%5, 0, 3);
                        
                        insert_ship(&left_ships, new_ship, renderer);
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;  // Type 3 (Patrol), Direction 0 (right to left)
                        break;
                    case SDLK_w:  // Fishing left
                        new_ship = create_ship(ships_created+1, 3, rand()%5, 0, 2);
                        
                        insert_ship(&left_ships, new_ship, renderer);
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;  // Type 2 (Fishing), Direction 0
                        break;
                    case SDLK_e:  // Normal left
                        new_ship = create_ship(ships_created+1, rand()%2+1, rand()%5, 0, 1);
                        
                        insert_ship(&left_ships, new_ship, renderer);
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;  // Type 0 (Normal), Direction 0
                        break;
                    
                    // Ships right to left
                    case SDLK_i:  // Normal right
                        new_ship = create_ship(ships_created+1, rand()%2+1, rand()%5, 1, 1);
                        
                        insert_ship(&right_ships, new_ship, renderer);
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;  // Type 1 (Normal), Direction 1 (left to right)
                        break;
                    case SDLK_o:  // Fishing right
                        new_ship = create_ship(ships_created+1, 3, rand()%5, 1, 2);
                        
                        insert_ship(&right_ships, new_ship, renderer);
                        schedule_ships(&canal_config,&left_ships, &right_ships);
                        ships_created++;  // Type 2 (Fishing), Direction 1
                        break;
                    case SDLK_p:  // Patrol right
                        new_ship = create_ship(ships_created+1, 5, rand()%5, 1, 3);
                        
                        insert_ship(&right_ships, new_ship, renderer);  
                        schedule_ships(&canal_config,&left_ships, &right_ships);                     
                        ships_created++;  // Type 3 (Patrol), Direction 1
                        break;
                    case SDLK_s:
                        start_canal(&canal_config, &left_ships, &right_ships, renderer);
                        break;
                    default:
                        break;
                }
            }
        }
        
            
        SDL_RenderPresent(renderer);

        frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time){
            SDL_Delay(frame_delay - frame_time);
        }

    }    

    // Liberar memoria de las listas de barcos
    Node *current;
    while (left_ships) {
        current = left_ships;
        left_ships = left_ships->next;
        free(current);
    }
    while (right_ships) {
        current = right_ships;
        right_ships = right_ships->next;
        free(current);
    }

    closeSDL(window, renderer);
    return 0;
}

int initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    *window = SDL_CreateWindow("Scheduling Ships Canal de Bolivia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create Render
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    return 0;
}

void closeSDL(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}