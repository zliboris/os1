#ifndef PROJECT_BASE_V1_1_MEM_H
#define PROJECT_BASE_V1_1_MEM_H

#include "../lib/hw.h"

#ifdef __cplusplus
extern "C" {
#endif

void* m_alcblokovi(size_t blokova);

void* m_alc(size_t Bytes);

int free_mem(void* p);

void initMem();

void printmem();

#ifdef __cplusplus
}
#endif


#endif //PROJECT_BASE_V1_1_MEM_H
