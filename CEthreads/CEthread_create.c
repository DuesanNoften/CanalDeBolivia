//
// Created by duesan on 30/09/24.
//

#include "CEthread.h"
int CEthread_create(CEthread *thread, void *(*start_routine)(void *), void *arg){
  CEthread_attr_t attr;
  CEthread_attr_init(&attr);
  CEthread_attr_setdetachstate(&attr, CETHREAD_CREATE_JOINABLE);
  CEthread_create(&thread->id, &attr, start_routine, arg);
  CEthread_attr_destroy(&attr);
  return 0;
  }


