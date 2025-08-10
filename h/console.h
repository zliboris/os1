//
// Created by os on 5/19/24.
//

#ifndef PROJECT_BASE_V1_1_CONSOLE_H
#define PROJECT_BASE_V1_1_CONSOLE_H

#include "buffer.h"
#include "semaphore.h"

void put_c(char chr);

char get_c();

void console_h();

void innerThreadPutc(void* a);

void initConsole();

void closeConsole();

int printEnd();


#endif //PROJECT_BASE_V1_1_CONSOLE_H
