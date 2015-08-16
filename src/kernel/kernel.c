#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/io/fat12.h>
#include <usr/string.h>

void kmain(void)
{
	
	char buffer[15];
	
	init_terminal();

	fat12_load_bpb();
	floppy_rw_sectors(0, 33, 5, 1);
	floppy_rw_sectors(0, 60, 5, 0);
	
	while(1){	
		puts_attrib("$", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer);
		puts_attrib(buffer, color_entry(COLOR_RED, COLOR_BLACK));
		puts("\r\n");
		
	}
}
