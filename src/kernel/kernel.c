#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <usr/string.h>
void kmain(void)
{
	char buffer[15];
	
	init_terminal();
	while(1){	
		puts(">");
		get_string(buffer);

	
		puts("\r\n");
		
	}
}
