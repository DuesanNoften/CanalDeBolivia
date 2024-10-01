//
// Created by duesan on 30/09/24.
//
#include "CEthread.h"
void CEmutex_destroy (CEmutex *mutex){
  if (mutex != NULL){
    CEfree(mutex);
  }