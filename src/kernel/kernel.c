#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/io/fat12.h>
#include <kernel/asm/memory.h>
#include <string.h>

uint8_t buffer[300];
uint8_t b2[11];
uint16_t i = 0;
int8_t rc;
fat12_entry_t ent;
file_t fh;


void kmain(void)
{
	init_terminal();
	
	if((rc = init_fs()) > 0)
		puts("[KERNEL] FS INIT ERROR\r\n");

	memcpy((char*)&ent, "KERNEA  SYS", 11);

	//print_int(create_file("KERNEA  SYS", 0, 50), 10, 0);
	
	while(1){	
		puts_attrib("$", color_entry(COLOR_GREEN, COLOR_BLACK));
		get_string(buffer, 30, true);
		if(strcmp(buffer, "ls") == 0){
			print_files();
			continue;
		}
		if(strcmp(buffer, "kk") == 0){
			int8_t r;
			get_string(buffer, 11, false);
			r = create_file(buffer, 0, 30);
			if(r != DISK_OP_OK)
				print_int(r, 10, 0);
			
		}
		if(strcmp(buffer, "dd") == 0){
			int8_t r;
			get_string(buffer, 11, false);
			r = delete_file(buffer);
			if(r != DISK_OP_OK){
				print_int(r, 10, 0);
			}
		}
		
		//puts("\r\n");
	}
	
}

