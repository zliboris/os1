#include "../h/mem.h"
#include "../h/print.h"

static size_t br_blokova;
static uint32 head_mask;
static uint32 alloc_mask;
static uint32* memMap;
static void* start_addr;

int isHead(uint32 m){
    if((m & head_mask) > 0)return 1;
    return 0;
}

int isFree(uint32 m){
    if((m & alloc_mask) > 0) return 0;
    return 1;
}

uint32 removeMask(uint32 m){
    return m & ~(head_mask | alloc_mask);
}

void initMem()
{
    br_blokova = ( (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR ) / MEM_BLOCK_SIZE * 16 / 17 - 1;
    head_mask = 1 << 30;
    alloc_mask = 1 << 29;
    memMap = (uint32*) HEAP_START_ADDR;
    start_addr = (void*) ((uint64) HEAP_START_ADDR + (br_blokova * 4));
    if((uint64)start_addr % MEM_BLOCK_SIZE != 0){start_addr = (void*)((uint64) start_addr / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE); start_addr += MEM_BLOCK_SIZE;}
    memMap[0] = br_blokova | head_mask;
    for(int i = 1; i < br_blokova - 1; i++)memMap[i]=0;
    memMap[br_blokova - 1] = br_blokova;
}

void* m_alcblokovi(size_t blokova)
{
    return m_alc(blokova * MEM_BLOCK_SIZE);
}

void* m_alc(size_t Bytes){
    if(Bytes <= 0)return 0;
    size_t blocks =  Bytes / MEM_BLOCK_SIZE;
    if(Bytes % MEM_BLOCK_SIZE != 0 || blocks == 0)blocks++;
    uint32 i=0;
    while((removeMask(memMap[i]) < blocks || isFree(memMap[i]) != 1) && i < br_blokova) {
        i += removeMask(memMap[i]);
    }
    if(i == br_blokova) return 0;
    uint32 j = i;
    uint32 old = removeMask(memMap[i]);
    memMap[i] = blocks | head_mask | alloc_mask;
    if(blocks > 1) {
        i += blocks - 1;
        memMap[i] = blocks | alloc_mask;
    }
    if(old > blocks){
        i++;
        memMap[i] = (old - blocks) | head_mask;
        if(old - blocks > 1){
            i+=  (old - blocks - 1);
            memMap[i] = old - blocks;
        }
    }
    uint64 ret_addr =  ( (uint64)start_addr + (j * MEM_BLOCK_SIZE));

    return (void*) ret_addr;
}

int free_mem(void* p) {
    if(p == 0)return -3;
    if (p < start_addr || p > HEAP_END_ADDR)return -1;
    if ((uint64) ((uint8 *) p - (uint8 *) start_addr) % 64 != 0)return -1;
    int i = (int) ((uint8 *) p - (uint8 *) start_addr) / 64;
    if (memMap[i] == 0) return -1;
    if (!isHead(memMap[i]) || isFree(memMap[i])) return -1;
    memMap[i] = removeMask(memMap[i]) | head_mask;
    if (removeMask(memMap[i]) > 1)
        memMap[i + removeMask(memMap[i]) - 1] = removeMask(
                memMap[i + removeMask(memMap[i]) - 1]);
    if (i > 0 && isFree(memMap[i - 1])) {
        uint32 j = removeMask(memMap[i]) + removeMask(memMap[i - 1]);
        uint32 temp = removeMask(memMap[i - 1]);
        memMap[i - temp] = j | head_mask;
        if (temp > 1) memMap[i - 1] = 0;
        int pi = (int) (i + removeMask(memMap[i]) - 1);
        memMap[i] = 0;
        i = pi;
        memMap[i] = j;
    } else i += (int) (removeMask(memMap[i]) - 1);
    if (i < br_blokova - 1 && isFree(memMap[i + 1])) {
        uint32 j = removeMask(memMap[i]) + removeMask(memMap[i + 1]);
        uint32 temp = removeMask(memMap[i + 1]);
        memMap[i + temp] = j;
        if (temp > 1) memMap[i + 1] = 0;
        int pi = (int) (i - removeMask(memMap[i]) + 1);
        memMap[i] = 0;
        i = pi;
        memMap[i] = j | head_mask;
    }
    return 0;
}

void printmem()
{
    for (int i = 0; i < br_blokova; i++) {
            if (memMap[i] != 0) {
                printstring("i: ");
                printint(i);
                printstring(" memMap[i]: ");
                printint(removeMask(memMap[i]));
                printstring("\tisHead: ");
                printint(isHead(memMap[i]));
                printstring("\tisFree: ");
                printint(isFree(memMap[i]));
                printstring("\n");
           }
        }
}


