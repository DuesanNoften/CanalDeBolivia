#ifndef CANAL_H
#define CANAL_H

#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdatomic.h>
#include "CEthreads/CEthread.h"

typedef struct {
    int length; // Length of the canal
    int control_mode; // 0 = Fairness, 1 = Sign, 2 = Tico
    int W; // For Fairness mode
    int sign_time; // For Sign mode
    int current_side; // 0 = left, 1 = right, controls which side is moving
    CEmutex_t canal_lock; // Mutex to control access to the canal
} Canal;

#endif CANAL_H