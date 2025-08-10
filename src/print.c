//
// Created by os on 5/16/24.
//
#include "../h/print.h"
#include "../h/console.h"


void printint(int num)
{
    char digs[10] = {'0','1','2','3','4','5','6','7','8','9'};
    int a[16], i = 0;
    if(num < 0) {
        num = -num;
        put_c('-');
    }
    do
    {
        a[i++] = num % 10;
        num=num/10;
    }while(num > 0);
    i--;
    while(i >= 0)
    {
        put_c(digs[a[i]]);
        i--;
    }
}

void printstring(char* s)
{
    while(*s != '\0')
    {
        put_c(*s);
        s++;
    }
}

void printhex(uint64 num)
{

    char digs[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int a[16], i = 0;
    if(num < 0) {
        num = -num;
        put_c('-');
    }
    do
    {
        a[i++] = num % 16;
        num=num/16;
    }while(num > 0);
    i--;
    while(i >= 0)
    {
        put_c(digs[a[i]]);
        i--;
    }
}