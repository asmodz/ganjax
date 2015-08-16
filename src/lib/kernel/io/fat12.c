#include <kernel/io/fat12.h>
char floppy_rw_sectors(short offset_, short lba, short count, char read)
{
	if(_ready == 0)
		fatal_error_box(errmesg("I/O Not ready = ", 0));
    _asm
    {
        mov ax, RAM_SEGMENT
        mov es, ax
    }
    while(count--)
    {
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
                fatal_error_box(errmesg("Disk Read Err:", rcode));
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
                fatal_error_box(errmesg("Disk Write Err:", rcode));
            }
        }
        
        offset_ += fat12_bpb.bytes_per_sector;
        lba++; 
    }
    _asm
    {
        mov ax, cs
        mov es, ax
    }
    return 0;
}

char floppy_reset_drive(){
    short flags_reg;
    _asm
    {
        mov ah, 0x00
        mov al, 0x00 /** Numer stacji dyskietek, zakladam 0 **/
        int 0x13
        pushf
        pop flags_reg
    }
    if(CHECK_BIT(flags_reg, CARRY_FLAG)){
        fatal_error_box("[GANJA KERNEL] Reset Drive error...");
    }
    else return 0;
}

void fat12_list_files(){
	__segment entseg = 0x07e0;
	char __based(entseg) *entptr = _ent_offset;
	int z = 0;
	while(*entptr){
		fat12_entry_t file;
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
	_asm
	{
		mov ax, cs
		mov es, ax
	}
}

void fat12_init_filesystem(){
	fat12_load_bpb();
	fat12_load_fat();
	fat12_load_files();
}

void fat12_load_fat(){
	_fat_offset = 0;
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

static int fat12_compute_fat_lba(){
	return ( fat12_bpb.reserved_sectors + fat12_bpb.sectors_per_fat );
}

static int fat12_compute_ent_lba(){
	return ( fat12_bpb.number_of_fats * fat12_bpb.sectors_per_fat + fat12_bpb.reserved_sectors );
}

static int fat12_compute_ent_size(){
	return ( fat12_bpb.max_files * ENTRY_SIZE ) / fat12_bpb.bytes_per_sector;
}
