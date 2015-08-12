#include <kernel/io/io.h>

void putc(uint8_t c){
    asm volatile("int $0x10" : : "a"(0x0e00 | (char) c ));
}

void puts(const char *__s){
    while(*__s){
        putc(*__s);
        __s++;
    }
}

key_t getch() {
     key_t key;
     uint16_t key_byte;
     asm volatile("int $0x16" : : "a"(0x0000));
     asm volatile("movw %%ax, %0" : "=r"(key_byte) : : "ax");
     key.slot = key_byte;
     return key;
}


void get_string(char* __s){
    key_t key;
    while(1){
        key = getch();
        putc(key.byte.ascii);
        if(key.byte.code == KEY_ENTER){
            puts("Enterek\r\n");
        }

    }
}



uint16_t strlen(const char* __s){
    
    uint16_t length = 0;
    while(*__s){
        length++;
        __s++;
    }
    return length;
}
