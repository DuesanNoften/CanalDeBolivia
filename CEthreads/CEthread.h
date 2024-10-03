#ifndef CETHREADS_H
#define CETHREADS_H

#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>

// Define the CEthreads_create structure
typedef struct {
    int (*start_routine)(void *);
    void *arg;
    char *stack;
    size_t stack_size;
    pid_t thread_id;
    int is_finished;
} CEthreads_t;

// Function to create a thread
int CEthread_create(CEthreads_t *ce_thread);
void CEthread_join(CEthreads_t *ce_thread, int *retval);

// Function to end a thread
void CEthread_end(CEthreads_t *ce_thread);

#endif CETHREADS_H
