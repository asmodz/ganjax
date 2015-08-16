#ifndef GANJAX_IO_H
#define GANJAX_IO_H
#include <kernel/asm/keyboard_defs.h>
#include <kernel/asm/asmdefs.h>
#include <kernel/io/terminal.h>
#include <usr/string.h>

#define INPUT_STRING_MAX_LENGTH 15

void putc(char);
void puts(const char* __s);
void print_int(int i, char base, char attrib);
void puts_attrib(const char *__s, short attrib);

void write_string_at(const char *__s, short x, short y, char attribs);
void fatal_error_box(const char *__s);

void get_string(char* __s);
key_t  get_key(void);



#endif
