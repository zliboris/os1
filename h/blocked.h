//
// Created by os on 5/18/24.
//

#ifndef PROJECT_BASE_V1_1_BLOCKED_H
#define PROJECT_BASE_V1_1_BLOCKED_H
#include "semaphore.h"


int blocked_put(PCB* p, semaphore* sem);

PCB* blocked_get(semaphore * sem);

#endif //PROJECT_BASE_V1_1_BLOCKED_H
