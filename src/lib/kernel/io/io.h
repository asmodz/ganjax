#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <stdint.h>
#include <kernel/asm/asm_defs.h>
#include <kernel/asm/keyboard_defs.h>
#include <kernel/debug/instr_repeater.h>


void  puts(const char *__s);
void  putc(uint8_t c);
key_t getch();

void  get_string(char *__s);



uint16_t strlen(const char* __s);

#endif
