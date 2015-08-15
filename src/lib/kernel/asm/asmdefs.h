#ifndef GANJAX_ASMDEFS_H
#define GANJAX_ASMDEFS_H
struct regs_t
{
	short ax;
	short bx;
	short cx;
	short dx;
	short flags;
	short cs;
	short ds;
	short es;
	short ss;
};
typedef struct regs_t regs_t;
regs_t get_regs();
void   set_regs(regs_t regs);
#endif
