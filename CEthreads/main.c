#include "CEthreads/CEthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h> // Include stdbool.h for true and false

atomic_bool thread_done = false;
CEmutex_t my_mutex;

int thread_function(void *arg) {
    printf("Thread is running\n");
    CEmutex_lock(&my_mutex);
    sleep(5); // Sleep for 5 seconds to simulate work
    printf("Thread function completed\n");
    thread_done = true;
    CEmutex_unlock(&my_mutex);
    return 0;
}

int main() {
    CEthreads_t my_thread;

    my_thread.start_routine = thread_function;
    my_thread.arg = NULL;
    my_thread.stack_size = 1024 * 1024; // 1 MB stack

    if (CEmutex_init(&my_mutex) != 0) {
        printf("Failed to initialize mutex\n");
        return 1;
    }

    if (CEthread_create(&my_thread) == -1) {
        printf("Failed to create thread\n");
        return 1;
    }

    printf("Thread created with PID: %d\n", my_thread.thread_id);

    sleep(1); // Give the thread time to run

    // Check if the thread is still running
    char command[256];
    snprintf(command, sizeof(command), "ps -p %d", my_thread.thread_id);
    printf("Checking if thread is running:\n");
    system(command);

    // Wait for the thread to complete
    while (!thread_done) {
        sleep(1);
    }

    // End the thread
    CEthread_end(&my_thread);

    printf("Thread terminated\n");

    // Check if the thread is terminated
    printf("Checking if thread is terminated:\n");
    system(command);

    // Destroy the mutex
    CEmutex_destroy(&my_mutex);

    return 0;
}