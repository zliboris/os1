//
// Created by os on 5/18/24.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORE_H
#define PROJECT_BASE_V1_1_SEMAPHORE_H

#include "../lib/hw.h"
#include "../h/riscv.h"
#include "PCB.h"
#include "scheduler.h"

semaphore* createSemaphore(uint64 init);

int closeSemaphore(semaphore* sem);

int waitSemaphore(semaphore* sem);

int signalSemaphore(semaphore* sem);

int trywaitSemaphore(semaphore* sem);

int dwaitSemaphore(semaphore* sem,time_t T);

int removeThreadSem(PCB* p,semaphore* sem);

enum semf {WAIT = 1, SIGNAL = 2, YIELD = 3, TIME_SLEEP = 4};

void signalinnerSemaphore(semaphore* sem);

void waitinnerSemaphore(semaphore* sem);

void yieldInner();

#endif //PROJECT_BASE_V1_1_SEMAPHORE_H
