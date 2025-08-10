//
// Created by os on 5/18/24.
//

#include "../h/blocked.h"
#include "../h/mem.h"


PCB* blocked_get(semaphore * sem)
{
    if(sem == 0 || sem->headlist == 0)return 0;
    PCB* ret = sem->headlist->procces;
    blocked_list* temp = sem->headlist;
    sem->headlist = sem->headlist->next;
    free_mem(temp);
    if(ret == 0){abort();}
    ret->semaphore = 0;
    return ret;
}

int blocked_put(PCB* p,semaphore* sem)
{
    if(p == 0)return -2;
    p->semaphore = sem;
    blocked_list* novi = (blocked_list*) m_alc(sizeof(blocked_list));
    if(novi == 0){
        abort();
    }
    novi->procces = p;
    novi->next = 0;
    if(sem->headlist == 0)
    {
        sem->headlist = novi;
        return 0;
    }
    blocked_list *temp = sem->headlist;
    while(temp->next != 0)temp = temp->next;
    temp->next = novi;
    return 0;
}
