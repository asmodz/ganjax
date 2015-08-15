#ifndef GANJAX_IO_H
#define GANJAX_IO_H
#include <kernel/asm/keyboard_defs.h>
#include <kernel/io/terminal.h>
#define INPUT_STRING_MAX_LENGTH 15
char* itoa(int value, char* str, int base);
void putc(char);

void puts(const char* __s);
void print_int(int v, int base);
void get_string(char* __s);
key_t  get_key(void);


#endif
