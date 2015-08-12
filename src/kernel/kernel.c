#include <kernel/io/io.h>

SETUP_REGISTERS();
JMP_TO_KERNEL_LOOP();


void _Noreturn main()
{
	/*_nop_sled;
	_asmv
	(	
		".intel_syntax noprefix;"
		"mov ax, 0xb700;"
		"mov ds, ax;"
	);
	short* video_memory = (short*) 0x1000;
	*video_memory = 0xFaFa;
	_nop_sled;
	_asmv
	(	
		".intel_syntax noprefix;"
		"mov ax, 0x0050;"
		"mov ds, ax;"
	);*/
	
	puts("Hello world from kernel :)");
	while(1)
	{
	
		
	}
}
