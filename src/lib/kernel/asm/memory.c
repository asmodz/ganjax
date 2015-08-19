#include <kernel/asm/memory.h>
#include <kernel/io/fat12.h>
void write_data_area(const char* __m, uint16_t __n, uint16_t __o){
    uint16_t i =0;
    char __based(ram_area) *mem = get_data_offset() + __o;
    for(;i<__n;++i) mem[i] = __m[i];
}

