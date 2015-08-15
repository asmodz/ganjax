#include <kernel/io/io.h>
#include <kernel/io/terminal.h>

void kmain(void)
{
	short x = 0;
	char buffer[30];
	clrscr(' ', color_entry(COLOR_RED, COLOR_CYAN));
	
	while(1){	
		get_string(buffer);
		puts("\r\n");
		puts(buffer);
	}
}
