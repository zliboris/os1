//
// Created by os on 5/16/24.
//

#ifndef PROJECT_BASE_V1_1_PRINT_H
#define PROJECT_BASE_V1_1_PRINT_H

#include "../lib/hw.h"

#ifdef __cplusplus
extern "C" {
#endif


void printint(int num);

void printstring(char* s);

void printhex(uint64 num);

#ifdef __cplusplus
}
#endif



#endif //PROJECT_BASE_V1_1_PRINT_H
