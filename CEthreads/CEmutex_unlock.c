//
// Created by duesan on 30/09/24.
//
#include "CEthread.h"
void CEmutex_lock(CEmutex *mutex){
  mutex->lock = 1;
  }