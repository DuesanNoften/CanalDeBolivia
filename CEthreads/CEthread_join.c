//
// Created by duesan on 30/09/24.
//
#include "CEthread.h"
void CEthread_join(CEthread *thread){
  CEthread_join(*thread, NULL);
}