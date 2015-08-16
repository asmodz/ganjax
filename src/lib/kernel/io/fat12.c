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

void fat12_load_fat(){
	
}

void fat12_load_files(){
	
}


