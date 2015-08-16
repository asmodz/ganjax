#ifndef GANJAX_ASMDEFS_H
#define GANJAX_ASMDEFS_H

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var, pos)  ((var) |= (1<<(pos)))
#define CLEAR_BIT(var, pos) ((var)&=~(1<<(pos)))
#define TOGGLE_BIT(var, pos)((var)^= 1 << pos)

/** FLAGS register bits **/
#define CARRY_FLAG 0
#define PARITY_FLAG 2
#define ADJUST_FLAG 4
#define ZERO_FLAG 6
#define SIGN_FLAG 7
#define TRAP_FLAG 8
#define INTERRUPT_FLAG 9
#define DIRECTION_FLAG 10
#define OVERFLOW_FLAG 11
#define IOPL0_BIT 12
#define IOPL1_BIT 13

#endif
