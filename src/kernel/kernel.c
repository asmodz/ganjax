#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/io/fat12.h>
#include <string.h>

uint8_t buffer[15];
uint16_t i = 0;
int8_t rc;
fat12_entry_t ent;
file_t file_handler;


void kmain(void)
{
	init_terminal();
	
	if((rc = init_fs()) > 0)
		puts("[KERNEL] FS INIT ERROR\r\n");
	
	if(( rc = load_file("TEST    TXT", &file_handler)) > 0){
		puts("ERR:"); print_int(rc, 10, 0); eol();
	}

	while(1){	
		puts_attrib("$", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer);
		if(strcmp(buffer, "ls") == 0){
			print_files();
			continue;
		}
		
		//puts("\r\n");
	}
	
}

