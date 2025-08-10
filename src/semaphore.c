//
// Created by os on 5/18/24.
//
#include "../h/semaphore.h"
#include "../h/mem.h"
#include "../h/blocked.h"

extern void innerSemaphore(semaphore* sem,uint64 id);

static semaphore *semafori;


semaphore* createSemaphore(uint64 init)
{
    semaphore *new = m_alc(sizeof(semaphore));
    if(new == 0)abort();

    new->headlist = 0;
    new->val = init;
    new->next = 0;

    if(semafori == 0)
    {
        semafori = new;
        return new;
    }

    semaphore *temp = semafori;
    while(temp->next != 0)
    {
        temp = temp->next;
    }
    temp->next = new;

    return new;
}


int closeSemaphore(semaphore* sem)
{
    if(sem == 0)return -3;
    while(sem->headlist != 0)
    {
        put(sem->headlist->procces);
        sem->headlist->procces->retval = SEM_DEAD;
        sem->headlist = sem->headlist->next;
    }

    semaphore *temp = semafori,*prev = 0;
    while(temp)
    {
        if(temp == sem)
        {
            if(prev == 0)
            {
                semafori = semafori->next;
//                free_mem(temp); // semafori nisu alocirani zbog slucaja gde korisnik moze da napravi semafor sacuva njegovu rucku dealocira ga i alocira novi semafor sa sansom da se alocira na istoj memoriskoj lokaciji pa se ne moze razlikovati novoalociran semafor i stari zatvoreni semafor
                return 0;
            }
            prev->next = temp->next;
//            free_mem(temp); // kao i ovde
            return 0;
        }
        prev = temp;
        temp = temp->next;
    }

    return -1;
}

int waitSemaphore(semaphore* sem)
{
    if(sem == 0)return -3;

    int stop = 0;
    semaphore *tre = semafori;
    while(tre != 0)
    {
        if(tre == sem)
        {
            stop = 1;
            break;
        }
        tre = tre->next;
    }
    if(!stop) return SEM_DEAD;
    PCB* old = getRunning();
    sem->val = sem->val - 1;
    if(sem->val < 0){blocked_put(old,sem);}
    else {
        old->retval = NORMAL;
        put(old);
    }
    PCB* new = get();
    if(new == 0)abort();
    setRunning(new);
    if(old != new) {
        resettimeSliceCounter();
        contextSwitch(&old->con, &new->con);
    }

    semaphore *temp = semafori;
    while(temp != 0)
    {
        if(temp == sem)
        {
            return NORMAL;
        }
        temp = temp->next;
    }

    return SEM_DEAD;
}

int signalSemaphore(semaphore* sem)
{
    int stop = 0;
    semaphore *tre = semafori;
    while(tre != 0)
    {
        if(tre == sem)
        {
            stop = 1;
            break;
        }
        tre = tre->next;
    }
    if(!stop) return SEM_DEAD;

    if(sem == 0)return -3;
    sem->val = sem->val + 1;
    if(sem->val <= 0){
        PCB *proces = blocked_get(sem);
        if(removeTimeList(proces) == 0)proces->retval = NORMAL;
        else proces->retval = TIMEOUT;
        put(proces);
    }
    PCB* old = getRunning();
    put(old);
    PCB* new = get();
    if(new == 0){abort();}
    setRunning(new);
    if(old != new) {
        resettimeSliceCounter();
        contextSwitch(&old->con, &new->con);
    }
    return 0;
}

int signalSemaphorenoyield(semaphore* sem) {
    if (sem == 0)return -3;
    sem->val = sem->val + 1;
    if (sem->val <= 0) put(blocked_get(sem));
    return 0;
}


int trywaitSemaphore(semaphore* sem)
{
    int stop = 0;
    semaphore *tre = semafori;
    while(tre != 0)
    {
        if(tre == sem)
        {
            stop = 1;
            break;
        }
        tre = tre->next;
    }
    if(!stop) return SEM_DEAD;

    if(sem == 0)return -3;
    PCB* old = getRunning();
    sem->val = sem->val - 1;
    if(sem->val < 0){ blocked_put(old,sem);}
    else return NORMAL;
    PCB* new = get();
    if(new == 0)abort();
    setRunning(new);
    if(old != new) {
        resettimeSliceCounter();
        contextSwitch(&old->con, &new->con);
    }

    semaphore *temp = semafori;
    while(temp != 0)
    {
        if(temp == sem)
        {
            return NORMAL;
        }
        temp = temp->next;
    }

    return SEM_DEAD;
}

int dwaitSemaphore(semaphore* sem,time_t T)
{
    int stop = 0;
    semaphore *tre = semafori;
    while(tre != 0)
    {
        if(tre == sem)
        {
            stop = 1;
            break;
        }
        tre = tre->next;
    }
    if(!stop) return SEM_DEAD;

    if(T == 0)return TIMEOUT;

    if(sem == 0)return -3;
    PCB* old = getRunning();
    sem->val = sem->val - 1;
    if(sem->val < 0) {
        if(blocked_put(old,sem) != 0)abort();
        put_timeList(old,T);
    }
    else {
        old->retval = NORMAL;
        put(old);
    }

    PCB* new = get();
    if(new == 0)abort();
    setRunning(new);
    if(old != new) {
        resettimeSliceCounter();
        contextSwitch(&old->con, &new->con);
    }

    return getRunning()->retval;
}

int removeThreadSem(PCB* p,semaphore* sem)
{
    blocked_list *prev = 0;
    blocked_list *temp = sem->headlist;
    while(temp)
    {
        if(temp->procces == p)
        {
            sem->val++;
            if(prev == 0)
            {
                sem->headlist = sem->headlist->next;
                free_mem(temp);
                return 0;
            }
            prev->next = temp->next;
            free_mem(temp);
        }
        prev = temp;
        temp = temp->next;
    }
    return -1;
}

void innerSemaphoreResolve(sacuvani_registri * registri)
{

    volatile uint64 sepc = r_sepc();
    volatile uint64 sstatus = r_sstatus();

    if(registri->a1 == WAIT)
    {
        trywaitSemaphore((semaphore*) registri->a0);
    }
    else if(registri->a1 == SIGNAL)
    {
        signalSemaphorenoyield((semaphore*) registri->a0);
    }
    else if(registri->a1 == YIELD)
    {
        resettimeSliceCounter();
        dispatch();
    }
    else if(registri->a1 == TIME_SLEEP)
    {
        resettimeSliceCounter();
        sleepTime((time_t) 1);
    }
    w_sstatus(sstatus);
    w_sepc(sepc);
}

void signalinnerSemaphore(semaphore* sem)
{
    volatile uint64 sstatus = r_sstatus();
    mc_sstatus(SSTATUS_SIE);

    innerSemaphore(sem,SIGNAL);

    ms_sstatus(sstatus & SSTATUS_SIE);
}

void waitinnerSemaphore(semaphore* sem)
{
    volatile uint64 sstatus = r_sstatus();
    mc_sstatus(SSTATUS_SIE);

    innerSemaphore(sem,WAIT);

    ms_sstatus(sstatus & SSTATUS_SIE);
}

void yieldInner()
{
    volatile uint64 sstatus = r_sstatus();
    mc_sstatus(SSTATUS_SIE);

    innerSemaphore(0,YIELD);

    ms_sstatus(sstatus & SSTATUS_SIE);
}