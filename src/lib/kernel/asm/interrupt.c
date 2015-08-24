#include <kernel/asm/interrupt.h>
#include <kernel/io/io.h>
void setup_interrupts()
{
	_asm
	{
		mov ax, 0x0000
		mov es, ax
		mov ax, int0h
		mov [es:0x0], al
		mov [es:0x1], ah
		mov [es:0x2], 0x50
		mov [es:0x3], 0x00
		mov ax, int1h
		mov [es:0x4], al
		mov [es:0x5], ah
		mov [es:0x6], 0x50
		mov [es:0x7], 0x00
		mov ax, int2h
		mov [es:0x8], al
		mov [es:0x9], ah
		mov [es:0xA], 0x50
		mov [es:0xB], 0x00
		mov ax, int3h
		mov [es:0x8], al
		mov [es:0x9], ah
		mov [es:0xA], 0x50
		mov [es:0xB], 0x00
		push cs
		pop es
	}
}

void _interrupt int0h(void){
	putc('a');
}

void _interrupt int1h(void){
	putc('b');
}

void _interrupt int2h(void){
	putc('c');
}

void _interrupt int3h(void){
	putc('d');
}




