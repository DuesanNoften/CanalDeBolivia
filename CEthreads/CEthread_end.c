#include <signal.h>
#include <stdlib.h>
#include "CEthread.h"

void CEthread_end(CEthreads_t *ce_thread) {
    // Send a signal to terminate the thread
    kill(ce_thread->thread_id, SIGKILL);

    // Free the allocated stack memory
    free(ce_thread->stack);
}