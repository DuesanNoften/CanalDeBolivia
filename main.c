#include "CEthreads/CEthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int thread_function(void *arg) {
    printf("Thread is running\n");
    CEthreads_t *ce_thread = (CEthreads_t *)arg;
    ce_thread->is_finished = 1;
    return 0;
}

int thread_function2(void *arg) {
    printf("Thread2 is running\n");
    return 0;
}

int main() {
    CEthreads_t my_thread;
    CEthreads_t my_thread2;
    my_thread.start_routine = thread_function;
    my_thread2.start_routine = thread_function2;
    my_thread.arg = NULL;
    my_thread2.arg = NULL;
    my_thread.stack_size = 1024 * 1024;
    my_thread2.stack_size = 1024 * 1024;// 1 MB stack
    my_thread.is_finished = 0;

    if (CEthread_create(&my_thread) == -1 || CEthread_create(&my_thread2) == -1) {
        printf("Failed to create thread\n");
        return 1;
    }


    int retval;
    CEthread_join(&my_thread, &retval);
    printf("Thread returned: %d\n", retval);
    return 0;

    sleep(1);// Give the thread time to run
    free(my_thread.stack);
    return 0;
}
