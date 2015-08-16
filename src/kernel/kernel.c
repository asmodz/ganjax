#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <usr/string.h>

void kmain(void)
{
	char buffer[15];

	init_terminal();
	rw_sectors(0, 33, 4, 1);
	rw_sectors(0, 60, 4, 0);
	


	while(1){	
		puts_attrib("$", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer);

		puts_attrib(buffer, color_entry(COLOR_RED, COLOR_BLACK));
		
		puts("\r\n");
		
	}
}
