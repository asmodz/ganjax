#include <kernel/asm/memory.h>
void write_data_area(const char* __m, uint16_t __n, uint16_t __o){
	uint16_t i =0;
	char __based(ram_area) *mem = __o;
	for(;i<__n;++i) mem[i] = __m[i];
}

