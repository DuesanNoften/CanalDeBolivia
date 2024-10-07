#include "Canal.h"
#include "CEthread.h"

void equidad_mode(Canal *canal, Queue *left_queue, Queue *right_queue) {
    int left_ships = W, right_ships = W;
    while (!queues_are_empty(left_queue, right_queue)) {
        // Allow W ships from left side
        if (left_ships > 0 && !queue_is_empty(left_queue)) {
            Ship *next_ship = dequeue(left_queue);
            CEthread_create(&ship_tid, NULL, ship_thread, (void *)next_ship);
            left_ships--;
        }

        // Once W ships have passed, switch to the right side
        if (left_ships == 0 && right_ships > 0 && !queue_is_empty(right_queue)) {
            switch_to_right_side();
            left_ships = W;
        }
    }
}