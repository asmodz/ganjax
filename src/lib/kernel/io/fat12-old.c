#include <kernel/io/fat12.h>

int8_t floppy_rw_sectors(uint16_t offset_, int16_t lba, uint16_t count, uint8_t read){
	if(_ready == 0) 
	{
		#ifdef EASY_GIVEUP
			fatal_error_box(errmesg("I/O Not ready = ", 0));
		#endif
		return DISK_NOT_READY;
	}
    _asm{
        mov ax, RAM_SEGMENT
        mov es, ax
    }
    while(count--){
        char c,h,s, rcode;
        /** Przelicz LBA na CHS **/
        c =       lba / ( fat12_bpb.sectors_per_track * fat12_bpb.heads_per_cylinder ); 
        h =     ( lba / fat12_bpb.sectors_per_track ) % fat12_bpb.heads_per_cylinder;
        s =     ( lba % fat12_bpb.sectors_per_track ) + 1;
        if(read)
        {
            _asm
            {
                mov ah, 0x02        /** Przerwanie czytaj sektory BIOSa **/
                mov al, 0x01        /** Czytaj jeden sektor **/
                mov ch, c           /** Koordy na dyskietce (CHS) **/
                mov dh, h
                mov cl, s
                mov dl, 0           /** Numer stacji dyskietek **/
                mov bx, offset_     /** Offset względem 0x07e0 **/
                int 0x13 
                mov rcode, ah
            }
            if(rcode != 0x00){
				#ifdef EASY_GIVEUP
					fatal_error_box(errmesg("Disk Read Err:", rcode));
				#endif
				return rcode;
            }
        }
        if(!read)
        {
            _asm
            {
                mov ah, 0x03        /** Przerwanie ppisz sektory BIOSa **/
                mov al, 0x01        /** pisz jeden sektor **/
                mov ch, c           /** Koordy na dyskietce (CHS) **/
                mov dh, h
                mov cl, s
                mov dl, 0           /** Numer stacji dyskietek **/
                mov bx, offset_     /** Offset względem 0x07e0 **/
                int 0x13 
                mov rcode, ah
            }   
            if(rcode != 0x00){
				#ifdef EASY_GIVEUP
					fatal_error_box(errmesg("Disk Write Err:", rcode));
				#endif
				return rcode;
            }
        }
        offset_ += fat12_bpb.bytes_per_sector;
        lba++; 
    }
    _asm{
        mov ax, cs
        mov es, ax
    }
    return DISK_OP_OK;
}

int8_t floppy_reset_drive(){
    int16_t flags_reg;
    _asm{
        mov ah, 0x00
        mov al, 0x00 /** Numer stacji dyskietek, zakladam 0 **/
        int 0x13
        pushf
        pop flags_reg
    }
    if(CHECK_BIT(flags_reg, CARRY_FLAG)){
		#ifdef EASY_GIVEUP
			fatal_error_box("[GANJA KERNEL] Reset Drive error...");
		#endif
    }
    else return DISK_UNABLE_RESET;
}

uint8_t fat12_create_new_entry(fat12_entry_t *ent){
	__segment entseg = 0x07e0;
	int8_t __based(entseg) *entptr = _ent_offset;
	int16_t files_count = 0;
	int8_t i=0;
	while(*entptr){
		files_count++;
		entptr += ENTRY_SIZE;
	}
	if(files_count == fat12_bpb.max_files - 1)
		return DISK_NO_SPACE_FOR_ENTRY;
		
	ent->extension[3] = ' ';
	for(i=0;i<ENTRY_SIZE;++i)
		entptr[i] = *((int8_t*)ent+i);
	return DISK_OP_OK;
}

uint16_t fat12_get_fat_entry(uint16_t clusternum){
    uint32_t offset;
    uint16_t value;
    uint8_t b1, b2;
    offset =  (3 * (clusternum/2));
    switch(clusternum % 2) 
    {
		case 0:
			b1 = *(fat_buffer + offset);
			b2 = *(fat_buffer + offset + 1);
			value = ((0x0f & b2) << 8) | b1;
		break;
		case 1:
			b1 = *(fat_buffer + offset + 1);
			b2 = *(fat_buffer + offset + 2);
			value = b2 << 4 | ((0xf0 & b1) >> 4);
		break;
    }
    return value;
}

void fat12_set_fat_entry(uint16_t clusternum, uint16_t value){
    uint32_t offset;
    char __based(ram_segment) *p1;
    char __based(ram_segment) *p2;
    offset = (3 * (clusternum/2));
    switch(clusternum % 2) 
    {
		case 0:
			p1 = fat_buffer + offset;
			p2 = fat_buffer + offset + 1;
			*p1 = (uint8_t)(0xff & value);
			*p2 = (uint8_t)((0xf0 & (*p2)) | (0x0f & (value >> 8)));
		break;
		case 1:
			p1 = fat_buffer + offset + 1;
			p2 = fat_buffer + offset + 2;
			*p1 = (uint8_t)((0x0f & (*p1)) | ((0x0f & value) << 4));
			*p2 = (uint8_t)(0xff & (value >> 4));
		break;
    }
}

void fat12_ls(){
	__segment entseg = 0x07e0;
	char __based(entseg) *entptr = _ent_offset;
	int z = 0;
	while(*entptr){
		fat12_entry_t file;
		if(*entptr == FAT12_REUSED){
			entptr += ENTRY_SIZE;
			continue;
		}
		for(z=0;z<ENTRY_SIZE;++z) ((char*)&file)[z] = entptr[z]; 
		entptr += ENTRY_SIZE;
		puts_attrib("Nazwa:", color_entry(COLOR_GREEN, COLOR_BLACK));
		puts_attrib(file.filename, color_entry(COLOR_CYAN, COLOR_BLACK));
		puts_attrib("Rozmiar:", color_entry(COLOR_RED, COLOR_BLACK));
		print_int(file.file_size, 10, color_entry( COLOR_LIGHT_RED, COLOR_BLACK));
		if(*entptr != 0) puts("\r\n");
	}
}


void fat12_load_bpb(){
	short  c, h, s, i;
	char err;
	char* fat12_bpb_ptr = ((char*)&fat12_bpb);
	__segment fat12_bpb_seg = 0x07e0;
	char __based(fat12_bpb_seg) *bpbptr = 0x03; // Skip JMP opcode
	_asm
	{
		mov ax, RAM_SEGMENT
		mov es, ax
		mov ah, 0x02
		mov al, 0x01
		mov ch, 0
		mov dh, 0
		mov cl, 1
		mov dl, 0
		mov bx, 0x0000
		int 0x13
		mov err, ah
	}
	if(err != 0x0) fatal_error_box(errmesg("FAT12 BPB Read error:", err));
	for(i=0;i<sizeof(fat12_bpb_t);++i) fat12_bpb_ptr[i] = bpbptr[i];
	_ready = 0xFF;
	_asm{
		mov ax, cs
		mov es, ax
	}
}

int8_t fat12_write_file(short offset_, fat12_entry_t  *ent){
	uint16_t needed = 0;
	uint16_t __based(ram_segment) *fat = 0x00;
	uint16_t cluster = first_free_cluster;
	uint16_t list_counter = 0;
	uint16_t i=0;
	uint8_t  rv;
	static uint16_t free_clusters_buffer[512]; 
	for(;i<512;++i) free_clusters_buffer[i] = 0;
	i = 0;
	if(first_free_cluster == 0)
		return DISK_NO_FREE_CLUSTERS;
		
	if(ent->file_size == 0)
		return DISK_BAD_SIZE;
	
	needed = (int) ent->file_size / (int) fat12_bpb.bytes_per_sector;
	if( (int) ent->file_size % fat12_bpb.bytes_per_sector > 0 )
		needed++;
	
	/** Szukaj wolnych klastrów **/
	for(;cluster < 9 * 512;++cluster){
		if( list_counter == needed )
			break;
		if( fat12_get_fat_entry(cluster) == FAT12_FREE )
			free_clusters_buffer[list_counter++] = cluster;
	}
	
	if( list_counter != needed )
		return DISK_NO_ENOUGH_CLUSTERS;
	
	/** OK, sektorów styka **/
	free_clusters_buffer[list_counter++] = FAT12_EOC;
	ent->first_cluster = free_clusters_buffer[0];
	rv = fat12_create_new_entry(ent);
	if(rv) return rv;
	/*puts("Wymagane sektory:"); print_int(needed, 10, 0); puts("\r\n");
	puts("Pierwszy wolny:"); print_int(first_free_cluster, 10, 0); puts("\r\n");
	puts("LC:"); print_int(list_counter, 10, 0); puts("\r\n");
	puts("Name:"); puts(ent->filename); puts("\r\n");*/	
	puts("Pierwszy wolny:"); print_int(first_free_cluster, 10, 0); puts("\r\n");
	if(needed == 1){
		fat12_set_fat_entry(free_clusters_buffer[0], FAT12_EOC);
		floppy_rw_sectors(offset_, fat12_cluster_to_lba(ent->first_cluster), 1, IO_WRITE);
	}
	else{
		for(;i<list_counter;++i){
			fat12_set_fat_entry(free_clusters_buffer[i], free_clusters_buffer[i+1]);
			if(free_clusters_buffer[i]!=FAT12_EOC)
				floppy_rw_sectors(offset_, fat12_cluster_to_lba(free_clusters_buffer[i]), 1, IO_WRITE);
			offset_ += fat12_bpb.bytes_per_sector;
		}			
	}
	/*puts("Wymagane sektory:"); print_int(needed, 10, 0); puts("\r\n");
	puts("Pierwszy wolny:"); print_int(first_free_cluster, 10, 0); puts("\r\n");
	puts("LC:"); print_int(list_counter, 10, 0); puts("\r\n");
	puts("Name:"); puts(ent->filename); puts("\r\n");
	for(;i<list_counter;++i){
		print_int(free_clusters_buffer[i], 10, 0);
		putc('(');
		print_int(first_free_cluster, 10, 0);
		putc(')');
		puts("\r\n");
	}*/
	fat12_write_fat();
	fat12_write_files();
	fat12_search_first_free_cluster();
	return DISK_OP_OK;
	
}

fat12_entry_t *fat12_search(const char* _x){
	__segment entseg = 0x07e0;
	char __based(entseg) *entptr = _ent_offset;
	static fat12_entry_t file;
	int z= 0;
	while(*entptr){
		memset((char*)&file, 0, sizeof(fat12_entry_t));
		for(z=0;z<ENTRY_SIZE;++z) ((char*)&file)[z] = entptr[z]; 
		entptr += ENTRY_SIZE;	
		if( strncmp(file.filename, _x, 11) == 0 )
			return &file;	
	}
	return 0;
}

void fat12_init_filesystem(){
	fat12_load_bpb();
	fat12_load_fat();
	fat12_search_first_free_cluster();
	fat12_load_files();
}

void fat12_load_fat(){
	_fat_offset = 0;
	fat_buffer = _fat_offset;
	floppy_rw_sectors(
		_fat_offset,
		fat12_compute_fat_lba(),		
		fat12_bpb.sectors_per_fat,		
		IO_READ
	);
}

void fat12_load_files(){
	_ent_offset = fat12_bpb.sectors_per_fat * fat12_bpb.bytes_per_sector;
	floppy_rw_sectors(
	 _ent_offset,
	 fat12_compute_ent_lba(),
	 fat12_compute_ent_size(),
	  IO_READ
	);
}

char __based(ram_segment)* fat12_load_file(uint16_t offset, const char* _name){
	int n;
	fat12_entry_t *file = fat12_search(_name);
	if(file != 0){
		uint16_t ret;
		uint16_t real_offset;
		n = file->first_cluster;
		// 0xAC00
		ret = real_offset = _ent_offset + offset + (fat12_compute_ent_size() * fat12_bpb.bytes_per_sector);
		while(n != FAT12_EOC){
			//print_int(n, 10, 0);
			//puts("\r\n");
			floppy_rw_sectors(real_offset, fat12_cluster_to_lba(n), 1, IO_READ);
			n = fat12_get_fat_entry(n);
			real_offset += fat12_bpb.bytes_per_sector;
		}
		return ret;
	}
	else 
		return WEEDFILE_NOEXISTS;
}

void fat12_write_fat(){
	floppy_rw_sectors(
		_fat_offset,
		fat12_compute_fat_lba(),
		9,
		IO_WRITE
	);
}

void fat12_write_files(){
	_ent_offset = fat12_bpb.sectors_per_fat * fat12_bpb.bytes_per_sector;
	floppy_rw_sectors(
		_ent_offset,
		fat12_compute_ent_lba(),
		fat12_compute_ent_size(),
		IO_WRITE
	);
}

static int fat12_cluster_to_lba(int cluster){
	return 33 + cluster - 2;
}

static int fat12_compute_fat_lba(){
	return ( fat12_bpb.reserved_sectors + fat12_bpb.sectors_per_fat );
}

static int fat12_compute_ent_lba(){
	return ( fat12_bpb.number_of_fats * fat12_bpb.sectors_per_fat + fat12_bpb.reserved_sectors );
}

static int fat12_compute_ent_size(){
	return ( fat12_bpb.max_files * ENTRY_SIZE ) / fat12_bpb.bytes_per_sector;
}

void fat12_print_file_clusters(uint16_t starring){
	int c = starring;
	while(c != FAT12_EOC){
		print_int(c, 10, 0);
		puts(" ");
		c = fat12_get_fat_entry(c);
	}
	puts("\r\n");
}

uint8_t fat12_delete_file(const char *_name){
	fat12_entry_t *ent =0;// fat12_search(_name);
	uint8_t __based(ram_segment) *entries = _ent_offset;
	uint16_t c = 0;
	while(*entries){
		int i=0;
		fat12_entry_t e;
		for(;i<32;++i)
			((char*)&e)[i] = entries[i];
		if(strncmp(e.filename, _name, 11) == 0){
			c = e.first_cluster;
			for(i=0;i<32;++i)
				entries[i] = FAT12_REUSED;
			break;
		}
		entries += 32;
	}
	if(c == 0)
		return DISK_FILE_NO_EXISTS;
	if(fat12_get_fat_entry(c) == FAT12_EOC){
		fat12_set_fat_entry(c, 0);
		return DISK_OP_OK;
	}
	while(c != FAT12_EOC){
		uint16_t next = fat12_get_fat_entry(c);
		fat12_set_fat_entry(c, FAT12_FREE);
		c = next;
	}
	fat12_search_first_free_cluster();
	fat12_write_fat();
	fat12_write_files();
	puts("First:");
	print_int(first_free_cluster, 10, 0);
	return DISK_OP_OK;
}

static void fat12_search_first_free_cluster(){
	uint16_t i = 2;
	uint16_t fat_size = fat12_bpb.sectors_per_fat * fat12_bpb.bytes_per_sector;
	for(;i < 9 * 512;++i){
		uint16_t pcluster = fat12_get_fat_entry(i);
		if(fat12_get_fat_entry(i) == FAT12_FREE){
			first_free_cluster = i;
			return;
		}
	}
	first_free_cluster = 0;
}

