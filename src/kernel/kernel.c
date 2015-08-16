#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/io/fat12.h>
#include <usr/string.h>


void kmain(void)
{
	
	char buffer[15];
	
	init_terminal();
	fat12_init_filesystem();

	
	while(1){	
		puts_attrib("$", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer);
		if(strcmp(buffer, "ls") == 0){
			fat12_list_files();
		}
		
		puts("\r\n");
		
	}
}
