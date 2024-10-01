//
// Created by duesan on 30/09/24.
//
#include "CEthread.h"
void CEthread_end(CEthread *thread){
  if(thread->status == CETHREAD_RUNNING){
    thread->status = CETHREAD_STOPPED;
  }else{
      thread->status = CETHREAD_STOPPED;
  }
}