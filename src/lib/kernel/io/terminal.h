#ifndef GANJAX_TERMINAL_H
#define GANJAX_TERMINAL_H
enum VIDEO_MODE{ 
	TEXT_40x25_16_8 = 0x00, 	/** 40x25   16 Colors 8 Pages **/
	TEXT_80x25_16_8 = 0x03, 	/** 80x25   16 Colors 8 Pages **/
	GRPH_320x200_256_1 = 0x13,	/** 320x200 16 Colors 1 Page  **/
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
	char row;
	char col;
};


typedef struct cursor_pos_t cursor_pos_t;
typedef enum VGA_COLOR  VGA_COLOR;
typedef enum VIDEO_MODE VIDEO_MODE;

static short VGA_WIDTH = 80;
static short VGA_HEIGHT = 25;
static cursor_pos_t cursor_position;
static VIDEO_MODE video_mode;


cursor_pos_t get_cursor_position();
void set_cursor_position(cursor_pos_t pos);

void set_video_mode(VIDEO_MODE mode);

void put_video_memory(char x, char y, short byte);
void clrscr(char chr, char color);
char color_entry(enum VGA_COLOR fg, enum VGA_COLOR bg);
short vga_entry(char c, char color);
#endif
