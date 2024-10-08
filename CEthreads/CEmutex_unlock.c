#include "CEthread.h"

void CEmutex_unlock(CEmutex_t *ce_mutex) {
    atomic_flag_clear(&ce_mutex->lock);
}