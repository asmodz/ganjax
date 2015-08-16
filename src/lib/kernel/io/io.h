#ifndef GANJAX_IO_H
#define GANJAX_IO_H
#include <kernel/asm/keyboard_defs.h>
#include <kernel/asm/asmdefs.h>
#include <kernel/io/terminal.h>
#include <usr/string.h>

#define INPUT_STRING_MAX_LENGTH 15

#define SECTORS_PER_TRACK 18
#define NUMBER_OF_HEADS 2
#define BYTES_PER_SECTOR 512
#define RAM_SEGMENT 0x07E0




void putc(char);
void puts(const char* __s);
void print_int(short i, char base);
void puts_attrib(const char *__s, short attrib);

void write_string_at(const char *__s, short x, short y, char attribs);
void fatal_error_box(const char *__s);

void get_string(char* __s);
key_t  get_key(void);

static char error_buffer[15];
static char code_buffer[4];

char rw_sectors(short offset_, short lba, short count, char read);
char reset_drive();

#endif
