#include "Canal.h"
#include "CEthread.h"


void letrero_mode(Canal *canal, Queue *left_queue, Queue *right_queue) {
    while (!queues_are_empty(left_queue, right_queue)) {
        // If the sign is on the left side, allow ships from the left queue
        if (canal->current_side == 0 && !queue_is_empty(left_queue)) {
            Ship *next_ship = dequeue(left_queue);
            CEthread_create(&ship_tid, NULL, ship_thread, (void *)next_ship);
        } else if (canal->current_side == 1 && !queue_is_empty(right_queue)) {
            // If the sign is on the right side, allow ships from the right queue
            Ship *next_ship = dequeue(right_queue);
            CEthread_create(&ship_tid, NULL, ship_thread, (void *)next_ship);
        }

        // After the sign's interval time, switch sides
        sleep(canal->sign_time);
        canal->current_side = !canal->current_side; // Toggle side
    }
}
