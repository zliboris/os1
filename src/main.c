#include "../lib/hw.h"
#include "../h/mem.h"
#include "../h/riscv.h"
#include "../h/PCB.h"
#include "../h/semaphore.h"
#include "../h/console.h"



extern void interrupt_wrapper();

extern void _Z8userMainv();

void wrapper(void* a)
{
    _Z8userMainv();
}

int main()
{
    initMem();


    PCB* Threads[2];

    Threads[0] = createThread(0,0);
    setRunning(Threads[0]);
    initConsole();
    w_stvec((uint64)&interrupt_wrapper);
    ms_sstatus(SSTATUS_SIE);

    Threads[1] = createInnerThread(innerThreadPutc,0);

    PCB* user;
    user = createThread(wrapper,0);


    while(user->finished != 1){yieldInner();}

//    printmem();

    while(!printEnd())yieldInner();


    mc_sstatus(SSTATUS_SIE);
    deleteScheduler();
    deletePCB(Threads[0]);
    setRunning(0);
    closeConsole();




    uint32 end = 0x5555;
    uint32* end_addr = (uint32*) 0x100000;
    *end_addr = end;

    return 0;
}

