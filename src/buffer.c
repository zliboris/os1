//
// Created by os on 5/19/24.
//

#include "../h/buffer.h"
#include "../h/mem.h"
#include "../h/print.h"
#include "../h/riscv.h"
#include "../h/semaphore.h"


buffer* init_buff(int _cap, int putc)
{
    buffer *buff = m_alc(sizeof(buffer));
    buff->cap = _cap + 1;
    buff->head = 0;
    buff->tail = 0;
    buff->buffer = m_alc(sizeof(char) * buff->cap);
    buff->putc = putc;
    buff->spaceleft = _cap;
    buff->itemAv = createSemaphore(0);
    if(buff->putc == 1)
        buff->spaceAv = createSemaphore(_cap);

    return buff;
}

void delete_buff(buffer *buff)
{
    if(buff->putc == 1)closeSemaphore(buff->spaceAv);
    closeSemaphore(buff->itemAv);
    free_mem(buff->buffer);
    free_mem(buff);
}

int put_buff(char c,buffer *buff)
{
    if(buff->putc == 1)waitinnerSemaphore(buff->spaceAv);
    buff->spaceleft--;
    buff->buffer[buff->tail] = c;
    buff->tail = (buff->tail + 1) % buff->cap;
    signalinnerSemaphore(buff->itemAv);
    return 0;
}

char get_buff(buffer *buff)
{
    waitinnerSemaphore(buff->itemAv);
    char ret = buff->buffer[buff->head];
    buff->head = (buff->head + 1) % buff->cap;
    if(buff->putc == 1)signalinnerSemaphore(buff->spaceAv);
    buff->spaceleft++;
    return ret;
}

int buff_full(buffer *buff)
{
    if(buff->spaceleft == 0)return 1;
    return 0;
}

int buff_empty(buffer* buff)
{
    if(buff->spaceleft == buff->cap - 1)return 1;
    return 0;
}
