#ifndef CETHREADS_H
#define CETHREADS_H

#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdatomic.h>

// Define CEthreads_create structure
typedef struct {
    int (*start_routine)(void *);
    void *arg;
    char *stack;
    size_t stack_size;
    pid_t thread_id;
    int is_finished;
} CEthreads_t;

// Define CEmutex_t structure using a spinlock
typedef struct {
    atomic_flag lock;
} CEmutex_t;

// Function to create a thread
int CEthread_create(CEthreads_t *ce_thread);

// Function to join a thread
void CEthread_join(CEthreads_t *ce_thread, int *retval);

// Function to end a thread
void CEthread_end(CEthreads_t *ce_thread);

// Function to initialize a mutex
int CEmutex_init(CEmutex_t *ce_mutex);

// Function to lock a mutex
void CEmutex_lock(CEmutex_t *ce_mutex);

// Function to unlock a mutex
void CEmutex_unlock(CEmutex_t *ce_mutex);

// Function to destroy a mutex
void CEmutex_destroy(CEmutex_t *ce_mutex);

#endif
