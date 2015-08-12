#ifndef ASM_DEFS_H
#define ASM_DEFS_H

#define __INTEL__ ".intel_syntax;"

#define put_ax_mem(x) __asm__ ("movw %0, %%ax" : : "r"(x) : "ax");
#define put_bx_mem(x) __asm__ ("movw %0, %%bx" : : "r"(x) : "bx");
#define put_cx_mem(x) __asm__ ("movw %0, %%cx" : : "r"(x) : "cx");
#define put_dx_mem(x) __asm__ ("movw %0, %%dx" : : "r"(x) : "dx");

#define put_ax_val(x) __asm__ ("movw %0, %%ax" : : "i"(x) : "ax");
#define put_bx_val(x) __asm__ ("movw %0, %%bx" : : "i"(x) : "bx");
#define put_cx_val(x) __asm__ ("movw %0, %%cx" : : "i"(x) : "cx");
#define put_dx_val(x) __asm__ ("movw %0, %%dx" : : "i"(x) : "dx");

#define _nop_sled _asm("nop");_asm("nop");_asm("nop");_asm("nop");

#define SETUP_REGISTERS() asm ( \
    __INTEL__ \
    "mov ax, cs;" \
    "mov ds, ax;" \
    "mov fs, ax;" \
    "mov gs, ax;" \
    "mov ax, 0x0000;" \
    "mov ss, ax;" \
    "mov ax, 0xffff;" \
    "mov sp, ax;" \
)

#define JMP_TO_KERNEL_LOOP() asm ( \
    __INTEL__   \
    "jmp main;" \
);

#endif
