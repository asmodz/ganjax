#ifndef GANJAX_IO_H
#define GANJAX_IO_H
#include <stdint.h>
#include <string.h>
#include <kernel/asm/keyboard_defs.h>
#include <kernel/asm/asmdefs.h>
#include <kernel/io/terminal.h>

#define INPUT_STRING_MAX_LENGTH 15

void putc(uint8_t c);
void puts(const char* __s);
void print_int(int16_t i, uint8_t base, int8_t attrib);
void puts_attrib(const char *__s, int16_t attrib);
void eol();

void write_string_at(const char *__s, uint16_t x, uint16_t y, int8_t attribs);
void fatal_error_box(const char *__s);

void   get_string(char* __s);
key_t  get_key(void);

#endif
