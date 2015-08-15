#include <kernel/io/terminal.h>

char color_entry(enum VGA_COLOR fg, enum VGA_COLOR bg){
    return fg | bg << 4;
}

short vga_entry(char c, char color){
    return c | color << 8;
}

void set_video_mode(VIDEO_MODE mode){
    _asm{
        mov al, mode
        mov ah, 0
        int 0x10
    }
    switch(mode){
        case TEXT_40x25_16_8:
            VGA_WIDTH = 25;
            VGA_HEIGHT = 40;
            break;
        case TEXT_80x25_16_8:
            VGA_WIDTH = 80;
            VGA_HEIGHT = 25;
            break;
    }
    video_mode = mode;
}

VIDEO_MODE get_video_mode(){
    _asm{
        mov ah, 0x0f
        int 0x10
        mov video_mode, al
    }
    return video_mode;
}

void set_cursor_shape(char start_row, char end_row){
    _asm{
        mov ch, start_row
        mov cl, end_row
        mov ah, 0x01
        int 0x10
    }
}

void set_cursor_position(cursor_pos_t pos){
    char row, col;
    row = pos.row;
    col = pos.col;
    _asm{
        mov ah, 0x2
        mov dh, row
        mov dl, col
        mov bh, 0
        int 0x10
    }
    cursor_position.row = row;
    cursor_position.col = col;
}

cursor_pos_t get_cursor_position(){
    cursor_pos_t pos;
    char row, col;
    _asm{
        mov ah, 0x3
        mov bh, 0x0
        int 0x10
        mov row, dh
        mov col, dl
    }
    pos.row = row;
    pos.col = col;
    return pos;
}

void put_video_memory(char x, char y, short byte){
    short index = y * VGA_WIDTH + x;
    /** Adres tak naprawde to 0xb8000 **/
    /** Kompilator adresuje ds:[video_memory] **/
    short* video_memory = 0x0000;
    _asm{
        mov ax, 0xb800;
        mov ds, ax
    }
    video_memory[index] = byte;
    _asm{
        mov ax, cs
        mov ds, ax
    }
}

cursor_pos_t get_max_xy(){
	cursor_pos_t screen_size;
	screen_size.row = VGA_HEIGHT;
	screen_size.col = VGA_WIDTH;
	return screen_size;
}

void init_terminal(){
    set_video_mode(TEXT_80x25_16_8);
    set_cursor_shape(0xf | (1 << 5) , 0); /** Jeśli 5 bit ustawiony **/
                                          /** Kursor niewidoczny    **/
    cursor_position.row = 0;
    cursor_position.col = 0;
    set_cursor_position(cursor_position);
    clrscr(' ', color_entry(COLOR_WHITE, COLOR_BLACK));
}

void clrscr(char chr, char color){
    int x,y;
    x=y=0;
    for(y = 0;y < VGA_HEIGHT; y++){
        for(x = 0; x < VGA_WIDTH; x++){
            put_video_memory(x, y, vga_entry(chr, color));
        }
    }
}
