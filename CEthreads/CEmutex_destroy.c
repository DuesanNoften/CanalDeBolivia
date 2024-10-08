#include "CEthread.h"

void CEmutex_destroy(CEmutex_t *ce_mutex) {
    atomic_flag_clear(&ce_mutex->lock);
}