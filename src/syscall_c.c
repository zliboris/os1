//
// Created by os on 5/16/24.
//
#include "../h/syscall_c.h"
#include "../lib/console.h"

const int EOF = -1;

void* mem_alloc(size_t size)
{
    size_t blocks =  size / MEM_BLOCK_SIZE;
    if(size % MEM_BLOCK_SIZE != 0)blocks++;
    __asm__ volatile (
            "mv a1, %[size];"
            "li a0, 0x01;"
            "ecall"
            : : [size] "r"(size)
            );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (void*)ret;
}

int mem_free(void* p)
{
    __asm__ volatile (
            "mv a1, %[p];"
            "li a0, 0x02;"
            "ecall"
            : : [p] "r"(p)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg)
{
    volatile uint8* stack =(uint8*) mem_alloc(sizeof(uint8) * DEFAULT_STACK_SIZE);
    stack = &stack[DEFAULT_STACK_SIZE];
    __asm__ volatile ("mv a4, %[stack]" : : [stack] "r"(stack));
    __asm__ volatile ("mv a3, %[arg]" : : [arg] "r"(arg));
    __asm__ volatile ("mv a2, %[start_routine]" : : [start_routine] "r"(start_routine));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile (
            "li a0, 0x11;"
            "ecall"
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int thread_exit()
{
    __asm__ volatile (
            "li a0, 0x12;"
            "ecall"
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

void thread_dispatch()
{
    __asm__ volatile (
            "li a0, 0x13;"
            "ecall"
            );
}

int sem_open(sem_t* handle, unsigned init)
{
    __asm__ volatile ("mv a2, %[init]" : : [init] "r"(init));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile (
            "li a0, 0x21;"
            "ecall"
            );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int sem_close(sem_t handle)
{
    __asm__ volatile (
            "mv a1, %[handle];"
            "li a0, 0x22;"
            "ecall"
            : : [handle] "r"(handle)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int sem_wait(sem_t id)
{
    __asm__ volatile (
            "mv a1, %[id];"
            "li a0, 0x23;"
            "ecall"
            : : [id] "r"(id)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int sem_signal(sem_t id)
{
    __asm__ volatile (
            "mv a1, %[id];"
            "li a0, 0x24;"
            "ecall"
            : : [id] "r"(id)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int sem_timedwait(sem_t id, time_t timeout)
{
    __asm__ volatile ("mv a2, %[timeout]" : : [timeout] "r"(timeout));
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile (
            "li a0, 0x25;"
            "ecall"
            );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int sem_trywait(sem_t id)
{
    __asm__ volatile (
            "mv a1, %[id];"
            "li a0, 0x26;"
            "ecall"
            : : [id] "r"(id)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

int time_sleep (time_t t)
{
    __asm__ volatile (
            "mv a1, %[t];"
            "li a0, 0x31;"
            "ecall"
            : : [t] "r"(t)
    );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return (int)ret;
}

char getc ()
{
    __asm__ volatile (
            "li a0, 0x41;"
            "ecall"
            );
    uint64 ret;
    __asm__ volatile("mv %[ret], a0" :  [ret] "=r"(ret));
    return ret;
}

void putc (char c)
{

    __asm__ volatile (
            "mv a1, %[c];"
            "li a0, 0x42;"
            "ecall"
            : : [c] "r"(c)
    );
}