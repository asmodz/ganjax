#include <kernel/io/io.h>
#include <kernel/io/terminal.h>

void kmain(void)
{
	int x = 300;
	char buffer[30];
	init_terminal();

	while(1){	
		get_string(buffer);
		
		puts(buffer);
		puts("\r\n");
	}
}
