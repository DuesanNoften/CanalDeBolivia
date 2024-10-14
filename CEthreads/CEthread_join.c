#include <unistd.h>
#include "CEthread.h"
void CEthread_join(CEthreads_t *ce_thread, int *retval){
  while (!ce_thread->is_finished) {
    usleep(100); // Sleep for a short period to avoid busy waiting
  }
  if (retval) {
    *retval = 0; // Assuming the return value is not used
  }
  free(ce_thread->stack);
}
