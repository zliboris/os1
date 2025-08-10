//
// Created by os on 5/17/24.
//

#ifndef PROJECT_BASE_V1_1_PCB_H
#define PROJECT_BASE_V1_1_PCB_H

#include "../lib/hw.h"
#include "strukture.h"

PCB* createThread(Body body, void* arg);

int thread_creat(PCB** thr,Body body, void* arg,void* stack);

PCB* createInnerThread(Body body, void* arg);

void yield();

void dispatch();

extern void contextSwitch(Context* old,Context* running);

extern void contextJump(Context* running);

void setRunning(PCB* r);

PCB* getRunning();

void deletePCB(PCB* p);

void threadWrapper();

void innerthreadWrapper();

void inctimeSliceCounter();

void resettimeSliceCounter();

uint64 gettimeSliceCounter();

int sleepTime(time_t T);

#endif //PROJECT_BASE_V1_1_PCB_H
