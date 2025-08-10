#ifndef PROJECT_BASE_V1_1_RISCV_H
#define PROJECT_BASE_V1_1_RISCV_H

#include "../lib/hw.h"
#include "print.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct element_registri
{
    uint64 zero;
    uint64 ra;
    uint64 sp;
    uint64 gp;
    uint64 tp;
    uint64 t0;
    uint64 t1;
    uint64 t2;
    uint64 s0;
    uint64 s1;
    uint64 a0;
    uint64 a1;
    uint64 a2;
    uint64 a3;
    uint64 a4;
    uint64 a5;
    uint64 a6;
    uint64 a7;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
    uint64 t3;
    uint64 t4;
    uint64 t5;
    uint64 t6;
}__attribute__ ((packed)) sacuvani_registri;

void popSppSpie();

inline uint64 r_scause()
{
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause , %[scause]" : : [scause] "r"(scause));
}

inline uint64 r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 r_stval()
{
    uint64 volatile stval;
    __asm__ volatile("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

enum BitMaskSip
{
    SIP_SSIP = (uint64)(1 << 1),
    SIP_STIP = (uint64)(1 << 5),
    SIP_SEIP = (uint64)(1 << 9),
};

inline uint64 r_sip()
{
    uint64 volatile sip;
    __asm__ volatile("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void ms_sip(uint64 mask)
{
    uint64 volatile sip = r_sip();
    sip = sip | mask;
    w_sip(sip);
}

inline void mc_sip(uint64 mask)
{
    uint64 volatile sip = r_sip();
    sip = sip & ~mask;
    w_sip(sip);
}

enum BitMaskSstatus
{
    SSTATUS_SIE = (uint64)(1 << 1),
    SSTATUS_SPIE = (uint64)(1 << 5),
    SSTATUS_SPP = (uint64)(1 << 8),
};

inline uint64 r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void ms_sstatus(uint64 mask)
{
    uint64 volatile sstatus = r_sstatus();
    sstatus = sstatus | mask;
    w_sstatus(sstatus);
}

inline void mc_sstatus(uint64 mask)
{
    uint64 volatile sstatus = r_sstatus();
    sstatus = sstatus & ~mask;
    w_sstatus(sstatus);
}

void abort();

#ifdef __cplusplus
}
#endif


#endif //PROJECT_BASE_V1_1_RISCV_H
