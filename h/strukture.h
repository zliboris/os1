//
// Created by os on 5/30/24.
//

#ifndef PROJECT_BASE_V1_1_STRUKTURE_H
#define PROJECT_BASE_V1_1_STRUKTURE_H


typedef void (*Body)(void*);

struct element_blocked;

typedef struct element_context
{
    uint64 ra;
    uint64 sp;
}__attribute__ ((packed)) Context;

enum returnValue
{
    SEM_DEAD = -1,
    TIMEOUT = -2,
    NORMAL = 0,
};

typedef struct element_semaphore
{
    int val;
    struct element_blocked* headlist;
    struct element_semaphore* next;
}semaphore;

typedef struct element_PCB {
    Body body;
    uint8* stack;
    Context con;
    uint64 timeSlice;
    void* args;
    uint8 finished;
    int retval;
    semaphore *semaphore;
}PCB;

typedef struct element_blocked
{
    PCB *procces;
    struct element_blocked* next;
}blocked_list;

typedef struct element_scheduler
{
    PCB* pcb;
    struct element_scheduler* next;
}scheduler_list;

typedef struct element_time
{
    PCB* pcb;
    time_t relativeT;
    struct element_time* next;
    struct element_time* prev;
}scheduler_time;

typedef struct element_buffer {
    int cap;
    char *buffer;
    int head, tail;
    int putc;
    int spaceleft;
    semaphore* spaceAv,*itemAv;
}buffer;





#endif //PROJECT_BASE_V1_1_STRUKTURE_H
