#ifndef GANJAX_EXEC_H
#define GANJAX_EXEC_H
#include <stdint.h>
#include <kernel/io/fat12.h>
#include <kernel/io/io.h>

#define MAX_BIN_SIZE      1024
#define EXEC_OP_OK        0x00
#define EXEC_SIZE_TOO_BIG 0x15
/** ========================================== **/

static uint8_t bin_buffer[MAX_BIN_SIZE];
static uint8_t return_code;
static file_t  file_handler;
static int (*raw_bin)();
   
/** ========================================== **/
void init_exec();
uint8_t run_program(const char *__n);
uint8_t get_return_code();
/** ========================================== **/
static void load_bin(uint16_t offset, uint16_t bin_size);
#endif
