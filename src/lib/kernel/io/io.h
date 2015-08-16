#ifndef GANJAX_IO_H
#define GANJAX_IO_H
#include <kernel/asm/keyboard_defs.h>
#include <kernel/asm/asmdefs.h>
#include <kernel/io/terminal.h>


#define INPUT_STRING_MAX_LENGTH 15

#define BASE_OFFSET 0x7900
#define SECTORS_PER_TRACK 18
#define NUMBER_OF_HEADS 2
#define BYTES_PER_SECTOR 512

#define RAM_SEGMENT 0x07e0



struct chs_t{
	char c,h,s;
};

typedef struct chs_t chs_t;

void putc(char);
void puts(const char* __s);
void puts_attrib(const char *__s, short attrib);

void write_string_at(const char *__s, short x, short y, char attribs);
void fatal_error_box(const char *__s);

void get_string(char* __s);
key_t  get_key(void);

chs_t lba_to_chs(short lba);

char read_sectors(short offset_, short lba, short count);

#endif
