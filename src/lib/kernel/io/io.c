#include <kernel/io/io.h>


void putc(char c){
    _asm{
        mov ah, 0x0e
        mov al, c
        int 0x10
    }
}

void puts(const char* __s){
    while(*__s != 0){
        putc(*__s);
        __s++;
    }
}

void puts_attrib(const char *__s , short attrib){
    cursor_pos_t pos = get_cursor_position();
    while(*__s){
        put_video_memory(pos.col, pos.row, vga_entry(*__s, attrib));
        pos.col += 1;
        set_cursor_position(pos);
        __s++;
    }
}

void get_string(char* __s){
    key_t k;
    short entry;
    cursor_pos_t pos;
    while(1){
        k = get_key();
        pos = get_cursor_position();
        if(k.byte.scan != KEY_ENTER && k.byte.scan != KEY_BACKSPACE && pos.col != INPUT_STRING_MAX_LENGTH){
            entry = vga_entry( k.byte.ascii, color_entry(COLOR_WHITE, COLOR_BLACK) );
            put_video_memory(pos.col, pos.row, entry);
            pos.col += 1;
            set_cursor_position(pos);
            *__s = k.byte.ascii;
            __s++;
        }
        else if(k.byte.scan == KEY_ENTER){
            *__s = '\0';
            puts("\r\n");
            return;
        }
        else if(k.byte.scan == KEY_BACKSPACE && pos.col != 1){
            __s--;
            *__s = '\0';
            entry = vga_entry( ' ', color_entry(COLOR_WHITE, COLOR_BLACK));
            pos.col -= 1;
            set_cursor_position(pos);
            put_video_memory(pos.col, pos.row, entry);
        }
    }
}

void print_regs(regs_t regs){
	char rn[3] = "\r\n";
	char buff[15];
	puts_attrib("AX:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.ax, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("BX:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.bx, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("CX:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.cx, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("DX:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.dx, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("CS:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.cs, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("DS:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.ds, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("SS:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.ss, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("ES:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.es, buff,  16), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
	
	puts_attrib("FL:", color_entry(COLOR_RED, COLOR_BLACK));
	puts_attrib(itoa(regs.flags, buff,  2), color_entry(COLOR_CYAN, COLOR_BLACK));puts(rn);
}

void write_string_at(const char *__s, short x, short y, char attribs){
	while(*__s){
		put_video_memory(x,y , vga_entry(*__s, attribs));
		x++;
		__s++;
	}
}

void fatal_error_box(const char *__s){
	cursor_pos_t scrsize = get_max_xy();
	clrscr(' ', color_entry(COLOR_BLACK, COLOR_BLUE));
	write_string_at(__s, (scrsize.col / 2) - strlen(__s) / 2, scrsize.row / 2, color_entry(COLOR_RED, COLOR_BLUE));
	while(1);
}

chs_t lba_to_chs(short lba){
    chs_t chs;
    chs.s = ( lba % SECTORS_PER_TRACK ) + 1;
    chs.h = ( lba / SECTORS_PER_TRACK ) % NUMBER_OF_HEADS;
    chs.c =   lba / ( SECTORS_PER_TRACK * NUMBER_OF_HEADS ); 
    return chs;
}

char read_sectors(short offset_, short lba, short count)
{
    while(count--)
    {
        char c,h,s;
        short flags_reg;
        short offst;
        offst = offset_ + BASE_OFFSET;
        c =       lba / ( SECTORS_PER_TRACK * NUMBER_OF_HEADS ); 
        h =     ( lba / SECTORS_PER_TRACK ) % NUMBER_OF_HEADS;
        s =     ( lba % SECTORS_PER_TRACK ) + 1;
        _asm
        {
            mov ah, 0x02
            mov al, 0x01
            mov ch, c
            mov dh, h
            mov cl, s
            mov dl, 0
            mov bx, offst;
            int 0x13 
            pushf
            pop flags_reg;
        }
        if(CHECK_BIT(flags_reg, CARRY_FLAG) ){
            fatal_error_box("[GANJA KERNEL] Disk Read Error...");
			return 1;
        }
        else
        {
            offset_ += BYTES_PER_SECTOR;
            lba++;
        }
    }
    return 0;
}

key_t get_key(void){
    key_t key;
    int kv = 0;
    _asm{
        mov ax, 0x0000
        int 0x16
        mov kv, ax
    }
    key.k = kv;
    return key;
}
