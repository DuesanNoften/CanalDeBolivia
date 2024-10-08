#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "CEthread.h"

int CEthread_create(CEthreads_t *ce_thread) {
    // Allocate memory for the thread's stack
    ce_thread->stack = malloc(ce_thread->stack_size);
    if (ce_thread->stack == NULL) {
        return -1;
    }

    // Create the thread using clone and store the thread ID
    ce_thread->thread_id = clone(
        (int (*)(void *))ce_thread->start_routine,
        ce_thread->stack + ce_thread->stack_size,
        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD,
        ce_thread->arg
    );

    // Check if clone was successful
    if (ce_thread->thread_id == -1) {
        free(ce_thread->stack); // Free the allocated stack memory on failure
        return -1;
    }

    return 0;
}