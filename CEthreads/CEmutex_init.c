#include "CEthread.h"

// Function to initialize a mutex
int CEmutex_init(CEmutex_t *ce_mutex) {
    atomic_flag_clear(&ce_mutex->lock);
    return 0;
}