#include "CEthread.h"

void CEmutex_lock(CEmutex_t *ce_mutex) {
    while (atomic_flag_test_and_set(&ce_mutex->lock)) {
        // Busy-wait (spin) until the lock is acquired
    }
}