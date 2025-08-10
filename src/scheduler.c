//
// Created by os on 5/17/24.
//
#include "../h/scheduler.h"
#include "../h/mem.h"
#include "../h/print.h"
#include "../h/riscv.h"
#include "../h/console.h"

static scheduler_list* procces_list = 0;

void busy_wait(void* a)
{
    while(1);
}

void deleteScheduler()
{
    while(procces_list != 0 )
    {
        deletePCB(procces_list->pcb);
        scheduler_list *temp = procces_list->next;
        free_mem(procces_list);
        procces_list = temp;
    }
}

//void setList(scheduler_list *s)
//{
//    s->test1 = 2345734;
//    s->test2 = 9765289;
//    s->test3 = 6930458;
//}
//
//void checkList(scheduler_list *s)
//{
//    if(s->test1 == 2345734 &&
//    s->test2 == 9765289 &&
//    s->test3 == 6930458) return;
//    printstring("Gazenje memorija");
//    abort();
//}

PCB* get(){
    if(procces_list == 0) {
        abort();
        PCB *bw =(PCB*) m_alc(sizeof(PCB));
        if(bw == 0)abort();
        bw->body = busy_wait;
        bw->con.ra = (uint64) &threadWrapper;
        bw->args = 0;
        bw->timeSlice = DEFAULT_TIME_SLICE;
        bw->stack = (uint8*) m_alc(sizeof(uint8) * DEFAULT_STACK_SIZE);
        bw->con.sp = (uint64) &bw->stack[DEFAULT_STACK_SIZE];
        bw->finished = 1;
        if(bw->stack == 0)abort();
        return bw;
    }
//    scheduler_list* temp = procces_list;

//    while(temp != 0)
//    {
//        checkList(temp);
//        temp = temp->next;
//    }

    PCB* ret = procces_list->pcb;
    scheduler_list* temp = procces_list;
    procces_list = temp->next;
    free_mem(temp);
    return ret;
}

int put(PCB* p){
    if(p == 0) {return -2;}
    scheduler_list* novi = (scheduler_list*) m_alc(sizeof(scheduler_list));

    if(novi == 0)abort();
//    setList(novi);
    novi->pcb = p;
    novi->next = 0;
    if(procces_list == 0)
    {
        procces_list = novi;
        if(procces_list->pcb == 0){ printstring("18");  abort();  }
        return 0;
    }
    scheduler_list* temp = procces_list;

    while(temp->next != 0)
    {
        temp = temp->next;
    }
    temp->next = novi;
    return 0;
}

static scheduler_time *t_head = 0;

//void printTime()
//{
//    printstring("\nLista vremena:\n");
//    scheduler_time *temp = t_head;
//    while(temp)
//    {
//        printhex(temp->relativeT);
//        printstring("\n");
//        temp = temp->next;
//    }
//    printstring("Kraj lista vremena\n");
//}

int put_timeList(PCB* p, time_t T)
{
    if(p == 0)return -2;
    if(T <= 0)return -3;

    scheduler_time *novi = (scheduler_time*) m_alc(sizeof(scheduler_time));
    if(novi == 0)return -1;

    novi->pcb = p;
    novi->relativeT = T;
    novi->prev = 0;
    novi->next = 0;

    if(t_head == 0) {
        t_head = novi;
        return 0;
    }

    if(novi->relativeT < t_head->relativeT) {
        novi->next = t_head;
        t_head->prev = novi;
        t_head->relativeT -= novi->relativeT;
        t_head = novi;
        return 0;
    }

    scheduler_time *temp = t_head;
    time_t sum = temp->relativeT;

    while(temp->next != 0 && sum + temp->next->relativeT <= novi->relativeT) {
        temp = temp->next;
        sum += temp->relativeT;
    }

    novi->next = temp->next;
    novi->prev = temp;
    temp->next = novi;
    if(novi->next) novi->next->prev = novi;

    novi->relativeT = T - sum;
    if(novi->next) novi->next->relativeT -= novi->relativeT;

    return 0;
}


void updateTimeList()
{
    if(t_head == 0)return;

    t_head->relativeT--;
    if(t_head->relativeT <= 0)
    {
        scheduler_time *temp;
        while(t_head != 0 && t_head->relativeT <= 0)
        {
            temp = t_head->next;
            if(temp != 0) temp->prev = 0;
            if(t_head->pcb->semaphore)
            {
                removeThreadSem(t_head->pcb,t_head->pcb->semaphore);
                t_head->pcb->semaphore = 0;
                t_head->pcb->retval = TIMEOUT;
            }
            put(t_head->pcb);
            free_mem(t_head);
            t_head = temp;
        }
    }
}

int removeTimeList(PCB* p)
{
    if(p == 0)return -2;
    scheduler_time *temp = t_head;
    while(temp != 0)
    {
        if(temp->pcb == p)
        {
            if(temp == t_head)
            {
                t_head = t_head->next;
                free_mem(temp);
                return 0;
            }
            temp->prev->next = temp->next;
            if(temp->next != 0) {
                temp->next->prev = temp->prev;
                temp->next->relativeT += temp->relativeT;
            }
            free_mem(temp);
            return 0;
        }
        temp = temp->next;
    }
    return -1;
}

