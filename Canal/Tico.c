#include "Canal.h"
#include "CEthread.h"

void tico_mode(Canal *canal, Queue *left_queue, Queue *right_queue) {
    while (!queues_are_empty(left_queue, right_queue)) {
        if (!queue_is_empty(left_queue)) {
            Ship *next_ship = dequeue(left_queue);
            CEthread_create(&ship_tid, NULL, ship_thread, (void *)next_ship);
        } else if (!queue_is_empty(right_queue)) {
            Ship *next_ship = dequeue(right_queue);
            CEthread_create(&ship_tid, NULL, ship_thread, (void *)next_ship);
        }
        
        // Avoid collision, only one side can move at a time
        CEmutex_lock(&canal->canal_lock);
        sleep(1); // Simulate time to prevent collisions
        CEmutex_unlock(&canal->canal_lock);
    }
}
