#include <kernel/asm/memory.h>

void memset(void *__s, char __c, short __n){
	while(__n){
		*((char*)__s) = __c;
		*((char*)__s)--;
	}
}
