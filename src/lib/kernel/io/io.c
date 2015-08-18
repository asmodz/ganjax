#include <kernel/io/io.h>

void putc(uint8_t c){
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

void puts_attrib(const char *__s , int16_t attrib){
    cursor_pos_t pos = get_cursor_position();
    while(*__s){
        put_video_memory(pos.col, pos.row, vga_entry(*__s, attrib));
        pos.col += 1;
        set_cursor_position(pos);
        __s++;
    }
}

void print_int(int16_t i , uint8_t base, int8_t attrib){
    static char buffer[32];
    itoa(i, buffer, base);
    if(attrib != 0)
		puts_attrib(buffer, attrib);
	else
		puts(buffer);
}

void eol(){
	puts("\r\n");
}

void get_string(char* __s){
    key_t k;
    int16_t entry;
    cursor_pos_t pos;
    while(1){
        k = get_key();
        pos = get_cursor_position();
		/** UGLY **/
        if(
            k.byte.scan != KEY_ENTER && k.byte.scan != KEY_BACKSPACE && pos.col != INPUT_STRING_MAX_LENGTH &&
           (k.byte.scan > KEY_ESC && k.byte.scan < KEY_RSHIFT) | k.byte.scan == KEY_SPACE
          ){
            entry = vga_entry( k.byte.ascii, color_entry(COLOR_WHITE, COLOR_BLACK) );
            put_video_memory(pos.col, pos.row, entry);
            pos.col += 1;
            set_cursor_position(pos);
            *__s = k.byte.ascii;
            __s++;
        }
        else if(k.byte.scan == KEY_ENTER && pos.col > 1){
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

void write_string_at(const char *__s, uint16_t x, uint16_t y, int8_t attribs){
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
