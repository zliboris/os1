//
// Created by os on 5/19/24.
//
#include "../h/console.h"
#include "../h/semaphore.h"


static buffer *input, *output;



void put_c(char chr)
{
    put_buff(chr, output);
}

char get_c()
{
    return get_buff(input);
}

void innerThreadPutc(void* a)
{
    mc_sstatus(SSTATUS_SIE);
    volatile uint8 *s = (uint8*) CONSOLE_STATUS;
    volatile uint8 *ctdata = (uint8*) CONSOLE_TX_DATA;
    volatile char c;
    while(1) {
        c = get_buff(output);
        while((*s & CONSOLE_TX_STATUS_BIT) == 0);
        *ctdata = (uint8) c;
    }
}

int printEnd()
{
    return buff_empty(output);
}


void console_h()
{
    volatile uint8 *s = (uint8*) CONSOLE_STATUS;
    volatile uint8 *crdata = (uint8*) CONSOLE_RX_DATA;
    volatile char c = 1;
    while(((*s) & CONSOLE_RX_STATUS_BIT) != 0)
    {
        if(buff_full(input))return;
        while((*s & CONSOLE_RX_STATUS_BIT) == 0);
        c=*crdata;
        put_buff(c,input);
    }
}

void initConsole()
{
    input = init_buff(100,0);
    output = init_buff(100,1);
}

void closeConsole()
{
    delete_buff(input);
    delete_buff(output);
}