#ifndef GANJAX_TERMINAL_H
#define GANJAX_TERMINAL_H
#pragma off(unreferenced)
#include <stdint.h>

enum VIDEO_MODE{ 
    TEXT_40x25_16_8 = 0x00,     /** 40x25   16 Colors 8 Pages **/
    TEXT_80x25_16_8 = 0x03,     /** 80x25   16 Colors 8 Pages **/
    GRPH_320x200_256_1 = 0x13,  /** 320x200 16 Colors 1 Page  **/
};

enum VGA_COLOR {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

struct cursor_pos_t{
    uint8_t row;
    uint8_t col;
};


typedef struct cursor_pos_t cursor_pos_t;
typedef enum VGA_COLOR  VGA_COLOR;
typedef enum VIDEO_MODE VIDEO_MODE;
/** ================================================= **/

/** Domyślna wielkość okna terminala **/
static uint16_t VGA_WIDTH  = 80;
static uint16_t VGA_HEIGHT = 25;
/** Tryb video **/
static VIDEO_MODE video_mode;

/** ================================================= **/
cursor_pos_t get_cursor_position();
cursor_pos_t get_max_xy();
void set_cursor_position(cursor_pos_t pos);

VIDEO_MODE get_video_mode();
void set_video_mode(VIDEO_MODE mode);

void set_cursor_shape(uint8_t start_row, uint8_t end_row);
void put_video_memory(uint8_t x, uint8_t y, uint16_t byte);
void clrscr(uint8_t chr, uint8_t color);
void init_terminal();

uint8_t color_entry(enum VGA_COLOR fg, enum VGA_COLOR bg);
uint16_t vga_entry(uint8_t c, uint8_t color);
#endif
