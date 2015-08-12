#ifndef KEYBOARD_DEFS
#define KEYBOARD_DEFS
#include <stdint.h>

#define KEY_ESC 		0x00
#define KEY_BACKSPACE   0x0E
#define KEY_TAB			0x0F
#define KEY_ENTER       0x1C
#define KEY_CTRL        0x1D
#define KEY_LSHIFT      0x2A
#define KEY_RSHIFT      0x36
#define KEY_PRTSCR      0x37
#define KEY_ALT         0x38
#define KEY_SPACE       0x39
#define KEY_CAPS        0x3A
#define KEY_F1          0x3B
#define KEY_F2			0x3C
#define KEY_F3          0x3D
#define KEY_F4          0x3E
#define KEY_F5          0x3F
#define KEY_F6          0x40
#define KEY_F7          0x41
#define KEY_F8          0x42
#define KEY_F9          0x43
#define KEY_F10         0x44
#define KEY_NUM         0x45
#define KEY_SCRL        0x46
#define KEY_HOME        0x47
#define KEY_UP          0x48
#define KEY_PGUP        0x49
#define KEY_MINUS       0x4A
#define KEY_LEFT        0x4B
#define KEY_CENTER      0x4C
#define KEY_RIGHT       0x4D
#define KEY_PLUS        0x4E
#define KEY_END         0x4F
#define KEY_DOWN        0x50
#define KEY_PGDN        0x51
#define KEY_INS         0x52
#define KEY_DEL         0x53

union key_t {	
  uint16_t slot; 
  struct key_byte_t {
    char ascii, code;  
  } byte;
};

typedef union key_t key_t;

#endif
