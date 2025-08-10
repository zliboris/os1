//
// Created by os on 5/17/24.
//
#include "../h/scheduler.h"
#include "../h/mem.h"
#include "../h/print.h"
#include "../h/riscv.h"
#include "../h/semaphore.h"

static PCB* running = 0;


static uint64 timeSliceCounter = 0;

void dispatch(){
    PCB* old = running;
    if(old->finished == 0){ put(old);}
    else {
        deletePCB(old);
        running = get();
        if(running == 0)abort();
        contextJump(&running->con);
        return;
    }
    running = get();
    if(running == 0)abort();
    contextSwitch(&old->con, &running->con);
}

void inctimeSliceCounter()
{
    timeSliceCounter++;
}

void resettimeSliceCounter()
{
    timeSliceCounter = 0;
}

uint64 gettimeSliceCounter()
{
    return timeSliceCounter;
}


PCB* createThread(Body body, void* arg){
    PCB* novi_proces = (PCB*) m_alc(sizeof(PCB));
    if(novi_proces == 0)abort();
    novi_proces->body = body;
    novi_proces->timeSlice = DEFAULT_TIME_SLICE;
    novi_proces->finished = 0;
    novi_proces->retval = 0;
    novi_proces->semaphore = 0;
    if(body != 0)
    {
        novi_proces->args = arg;
        novi_proces->stack =(uint8*) m_alc(sizeof(uint8) * DEFAULT_STACK_SIZE);
        novi_proces->con.sp = (uint64) &(novi_proces->stack[DEFAULT_STACK_SIZE]);
        novi_proces->con.ra = (uint64) &threadWrapper;
        put(novi_proces);
        if(novi_proces->stack == 0) abort();
    }
    else
    {
        novi_proces->stack =(uint8*) 0;
        novi_proces->con.ra = (uint64) 0;
        novi_proces->con.sp = (uint64) 0;
    }
    return novi_proces;
}

struct _thread;
typedef struct _thread* thread_t;

int thread_creat(PCB** thr,Body body, void* arg,void* stack){
    PCB* novi_proces = (PCB*) m_alc(sizeof(PCB));
    if(novi_proces == 0)abort();
    novi_proces->body = body;
    novi_proces->timeSlice = DEFAULT_TIME_SLICE;
    novi_proces->finished = 0;
    novi_proces->retval = 0;
    novi_proces->semaphore = 0;
    if(body != 0)
    {
        novi_proces->args = arg;
        novi_proces->stack =(uint8*) (((uint64)stack ) - (sizeof(uint8) * DEFAULT_STACK_SIZE));
        novi_proces->con.sp = (uint64) &(novi_proces->stack[DEFAULT_STACK_SIZE]);
        novi_proces->con.ra = (uint64) &threadWrapper;
        put(novi_proces);
        if(novi_proces->stack == 0)abort();
    }
    else
    {
        novi_proces->stack =(uint8*) 0;
        novi_proces->con.ra = (uint64) 0;
        novi_proces->con.sp = (uint64) 0;
    }
    *thr = novi_proces;
    return 0;
}

PCB* createInnerThread(Body body, void* arg){
    PCB* novi_proc = (PCB*) m_alc(sizeof(PCB));
    if(novi_proc == 0)abort();
    novi_proc->body = body;
    novi_proc->con.ra = (uint64) &innerthreadWrapper;
    novi_proc->timeSlice = DEFAULT_TIME_SLICE * 100;
    novi_proc->retval = 0;
    novi_proc->semaphore = 0;
    novi_proc->args = arg;
    novi_proc->stack = (uint8*) m_alc(sizeof(uint8) * DEFAULT_STACK_SIZE);
    novi_proc->con.sp = (uint64) &(novi_proc->stack[DEFAULT_STACK_SIZE]);
    put(novi_proc);
    if(novi_proc->stack == 0) abort();
    novi_proc->finished = 0;
    return novi_proc;
}

void yield()
{
    __asm__ volatile (
            "li a0, 0x13;"
            "ecall"
            );
}

void setRunning(PCB* r){
    running = r;
}

PCB* getRunning(){
    return running;
}

void deletePCB(PCB* p)
{
    if(p->stack != 0)free_mem(p->stack);
    free_mem(p);
}

void threadWrapper()
{
    popSppSpie();
    running->body(running->args);
    running->finished = 1;
    yield();
}

void innerthreadWrapper()
{
    running->body(running->args);
    running->finished = 1;
    yieldInner();
}

int sleepTime(time_t T)
{
    if(T <= 0)return -2;
    PCB* old = running;
    if(put_timeList(old,T) == 0) {
        running = get();
        if (running == 0)abort();
        contextSwitch(&old->con, &running->con);
        return 0;
    }
    return -1;
}