//
// Created by os on 5/17/24.
//

#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"



void* operator new(size_t s) {
    return mem_alloc(s);
}

void operator delete (void* p) noexcept{
    mem_free(p);
}

Thread::Thread (void (*b)(void*), void* a) :myHandle(nullptr), body(b), arg(a)
{
    thread_create(&myHandle, body, arg);
}

Thread::~Thread ()
{
}


int Thread::start ()
{
    return thread_create(&myHandle,&Thread::wrapper,this);
}

void Thread::dispatch ()
{
    thread_dispatch();
}

int Thread::sleep (time_t t)
{
    time_sleep(t);
    return 0;
}

Thread::Thread ()
{
    myHandle = nullptr;
    body = nullptr;
    arg = nullptr;
}

void Thread::wrapper(void* a) {
    Thread* t = (Thread*) a;
    t->run();
}


Semaphore::Semaphore(unsigned int init): myHandle(nullptr) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {return sem_wait(myHandle);}

int Semaphore::signal() {return sem_signal(myHandle);}

int Semaphore::timedWait(time_t T) {return sem_timedwait(myHandle,T);}

int Semaphore::tryWait() {return sem_trywait(myHandle);}

void PeriodicThread::terminate() {period = 0;}

PeriodicThread::PeriodicThread(time_t period):Thread(periodwrapper, this) , period(period) {}

void PeriodicThread::periodwrapper(void *arg) {
    PeriodicThread *thread = (PeriodicThread*)arg;
    while(true)
    {
        if(thread->period == 0)thread_exit();

        thread->periodicActivation();
        time_sleep(thread->period);
    }
}

void Console::putc(char c) { ::putc(c);}

char Console::getc() {return ::getc();}