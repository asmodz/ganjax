#include <kernel/io/io.h>
#include <kernel/io/terminal.h>
#include <kernel/io/fat12.h>
#include <string.h>

uint8_t buffer[15];
uint16_t i = 0;
int8_t rc;
fat12_entry_t ent;
file_t fh;


void kmain(void)
{
	init_terminal();
	
	if((rc = init_fs()) > 0)
		puts("[KERNEL] FS INIT ERROR\r\n");
	if((rc = load_file("TEST    TXT", &fh)) > 0){
		puts("Error Loading!");
	}
	
	for(i=0;i<fh.entry.file_size - 30;++i) fh.data[i] = 'p';
	
	memcpy((char*)&ent, "KERNEA  SYS", 11);
	ent.file_size = fh.entry.file_size;
	if((rc = fat12_create_new_file(0,  &ent)) > 0){
		puts("ERR\r\n");
		print_int(rc, 10, 0);
	}
	
	print_cluster_list(ent.first_cluster);
	
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

