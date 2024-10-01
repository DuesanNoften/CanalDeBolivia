//
// Created by duesan on 30/09/24.
//
#include "CEthread.h"
int CEmutex_init(struct CEmutex *mutex){
  mutex->lock = 0;
  mutex->owner = 0;
  return 0;
  }