//
// Created by os on 5/19/24.
//

#ifndef PROJECT_BASE_V1_1_BUFFER_H
#define PROJECT_BASE_V1_1_BUFFER_H

#include "semaphore.h"
#include "strukture.h"

buffer* init_buff(int cap, int putc);

void delete_buff(buffer *buff);

int put_buff(char c,buffer *buff);

char get_buff(buffer *buff);

int buff_full(buffer *buff);

int buff_empty(buffer* buff);

#endif //PROJECT_BASE_V1_1_BUFFER_H
