#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/asm/asmdefs.h>
#include <usr/string.h>

void kmain(void)
{
	char buffer[15];
	regs_t regs;
	init_terminal();
	read_sectors(0, 1, 1);

	regs = get_regs();
	
	regs.ax = 10;
	print_regs(regs);
	while(1){	
		puts_attrib(">", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer);

		puts_attrib(buffer, color_entry(COLOR_RED, COLOR_BLACK));
		
		puts("\r\n");
		
	}
}
