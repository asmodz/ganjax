#include <kernel/io/io.h>
char * itoa( int value, char * str, int base ){
    char * rc;
    char * ptr;
    char * low;
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if ( value < 0 && base == 10 ){
        *ptr++ = '-';
    }
    low = ptr;
    do{
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    *ptr-- = '\0';
    while ( low < ptr ){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

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

void print_int(int v, int base){
	static char INT_CV_BUFFER[6];
	itoa(v, INT_CV_BUFFER, base);
	puts(INT_CV_BUFFER);
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
		else if(k.byte.scan == KEY_BACKSPACE && pos.col != 0){
			__s--;
			*__s = '\0';
			entry = vga_entry( ' ', color_entry(COLOR_WHITE, COLOR_BLACK));
			pos.col -= 1;
			set_cursor_position(pos);
			put_video_memory(pos.col, pos.row, entry);
		}
	}
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
