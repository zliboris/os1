#include "../lib/console.h"
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/riscv.h"
#include "../h/mem.h"
#include "../h/print.h"
#include "../h/PCB.h"
#include "../h/semaphore.h"
#include "../h/console.h"


struct _thread;
typedef struct _thread* thread_t;

struct _sem;
typedef struct _sem* sem_t;

void interrupt_handler(sacuvani_registri* registri)
{
    volatile uint64 scause = r_scause();
    volatile uint64 BNT = scause >> 63;
    volatile uint64 val = scause & ~((uint64)1 << 63);
    volatile uint64 sepc;
    volatile uint64 sstatus;
    if(BNT && val == 1)
    {
        // tajmer
        updateTimeList();
        if(getRunning() != 0) {
            inctimeSliceCounter();
            if (gettimeSliceCounter() >= getRunning()->timeSlice) {
                resettimeSliceCounter();
                sepc = r_sepc();
                sstatus = r_sstatus();
                dispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);

            }
        }
    }
    else if(BNT && val == 9) {

        // Spoljasnji hardverski prekid
        int interrpt = plic_claim();
        console_h();
        plic_complete(interrpt);
        mc_sip(SIP_SEIP);
    }
    else if(!BNT && val == 8) {
        // ecall iz korisnickog rezima
        switch(registri->a0) {
            case 0x01:
                //mem_alloc
                registri->a0 = (uint64) m_alcblokovi(registri->a1);
                break;
            case 0x02:
                //mem_free
                registri->a0 = free_mem((void *) registri->a1);
                break;
            case 0x11:
                // thread_create
                if(getRunning() != 0) {
                    thread_t *handle = (thread_t *) registri->a1;
                    if(handle == 0) {
                        registri->a0 = -2;
                        break;
                    }
                    thread_creat( (PCB**) handle,(Body) registri->a2, (void *) registri->a3,(void*) registri->a4);
                    if (*handle == 0) {
                        registri->a0 = -1;
                    } else {
                        registri->a0 = 0;
                    }
                }
                break;
            case 0x12:
                // thread_exit
                if(getRunning() != 0) {
                    getRunning()->finished = 1;
                    resettimeSliceCounter();
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    dispatch();
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;

            case 0x13:
                // thread_dispatch
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    resettimeSliceCounter();
                    dispatch();
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;
            case 0x21:
                // sem_open
                if(getRunning() != 0)
                {
                    sem_t *handle = (sem_t*) registri->a1;
                    if(handle == 0) {
                        registri->a0 = -2;
                        break;
                    }
                    semaphore *new = createSemaphore(registri->a2);
                    if(new == 0){
                        registri->a0 = -1;
                    } else {
                        *handle = (struct _sem*) new;
                        registri->a0 = 0;
                    }
                }
                break;
            case 0x22:
                // sem_close
                if(getRunning() != 0) {
                    if (closeSemaphore((semaphore *) registri->a1) != 0)registri->a0 = -1;
                    else registri->a0 = 0;
                }
                break;
            case 0x23:
                // sem_wait
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    registri->a0 = waitSemaphore((semaphore *) registri->a1);
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;
            case 0x24:
                // sem_signal
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    registri->a0 = signalSemaphore((semaphore *) registri->a1);
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;
            case 0x25:
                // sem_timedwait
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    registri->a0 = dwaitSemaphore((semaphore *) registri->a1, (time_t) registri->a2);
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;
            case 0x26:
                // sem_trywait
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    registri->a0 = trywaitSemaphore((semaphore *) registri->a1);
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                    registri->a0 = 0;
                }
                break;
            case 0x31:
                // time_sleep
                if(getRunning() != 0) {
                    sepc = r_sepc();
                    sstatus = r_sstatus();
                    resettimeSliceCounter();
                    registri->a0 = sleepTime((time_t) registri->a1);
                    w_sstatus(sstatus);
                    w_sepc(sepc);
                }
                break;
            case 0x41:
                //getc
                registri->a0 = get_c();
                break;
            case 0x42:
                //putc
                put_c((char)registri->a1);
                break;
        }
        w_sepc(r_sepc() + 4);
    }
    else if(!BNT && val == 9) {
        // ecall iz sistemskog rezima
        w_sepc(r_sepc() + 4);
    }
    else{
        // Ilegalna instrukcija | Nedozvoljena adresa citanja | Nedozvoljena adresa upisa
        printstring("\ninterrupt abort\n");
        abort();
    }
    mc_sip(SIP_SSIP);
}