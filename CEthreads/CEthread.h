#ifndef CETHREADS_H
#define CETHREADS_H

#include <sched.h>
#include <stdlib.h>

// Define the CEthreads_create structure
typedef struct {
    int (*start_routine)(void *);
    void *arg;
    char *stack;
    size_t stack_size;
    int is_finished
} CEthreads_t;

// Function to create a thread
int CEthread_create(CEthreads_t *ce_thread);
void CEthread_join(CEthreads_t *ce_thread, int *retval);

#endif CETHREADS_H
