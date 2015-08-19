#include <kernel/asm/exec.h>
/** ========================================== **/

uint8_t run_program(const char *__n){
    uint8_t rc;
    if((rc = load_file(__n, &file_handler)))
        return rc;
        
    if( file_handler.entry.file_size > MAX_BIN_SIZE )
        return EXEC_SIZE_TOO_BIG;
    
    puts_attrib("=================EXECUTOR=================", color_entry(COLOR_GREEN, COLOR_BLACK)); eol();
    load_bin(0, file_handler.entry.file_size);
    return_code = (int)(*raw_bin)();
    eol();
    puts_attrib("=================EXECUTOR=================", color_entry(COLOR_RED, COLOR_BLACK));
    eol(); 
    puts_attrib("RC:", color_entry(COLOR_MAGENTA, COLOR_BLACK)); print_int(return_code, 10, 0);eol();
    puts_attrib("=================EXECUTOR=================", color_entry(COLOR_GREEN, COLOR_BLACK));
    free_file();
    return EXEC_OP_OK;
}

/** ========================================== **/
static void load_bin(uint16_t offset, uint16_t bin_size){
    uint16_t i=0;
    for(;i<file_handler.entry.file_size;++i)
        bin_buffer[i] = file_handler.data[i];
    raw_bin = (int (*)())bin_buffer;
}
