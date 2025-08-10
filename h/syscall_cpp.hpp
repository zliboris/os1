//
// Created by os on 5/16/24.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#include "syscall_c.h"

struct _thread;
typedef struct _thread* thread_t;

struct _sem;
typedef struct _sem* sem_t;


void* operator new (size_t) noexcept(false);

void operator delete (void*) noexcept;
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    static void wrapper(void*);
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    sem_t myHandle;
};


class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
    static void periodwrapper(void* arg);
};
class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
