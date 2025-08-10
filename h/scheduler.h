//
// Created by os on 5/17/24.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H
#include "PCB.h"
#include "strukture.h"

void deleteScheduler();

PCB* get();

int put(PCB* p);

int put_timeList(PCB* p, time_t T);

void updateTimeList();

int removeTimeList(PCB* p);


#endif //PROJECT_BASE_V1_1_SCHEDULER_H
