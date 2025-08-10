//
// Created by os on 5/16/24.
//
#include "../h/riscv.h"
#include "../h/print.h"
#include "../h/console.h"
#include "../h/PCB.h"

void popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void abort()
{
    mc_sstatus(SSTATUS_SIE);
    printstring("\nssattus: ");
    printhex(r_sstatus());
    printstring("\nscause: ");
    printhex(r_scause());
    printstring("\nsepc: ");
    printhex(r_sepc());
    printstring("\n");

    while(!printEnd())yieldInner();

    uint32 end = 0x5555;
    uint32* end_addr = (uint32*) 0x100000;
    *end_addr = end;
}