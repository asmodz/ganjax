#include <kernel/asm/asmdefs.h>

regs_t get_regs(){
	regs_t regs;
	short _ax = 0, _bx = 0, _cx = 0, _dx = 0, _flags = 0, _cs = 0,
		  _ds = 0, _ss = 0, _es = 0;
	
	_asm{
		mov _ax, ax
		mov _bx, bx
		mov _cx, cx
		mov _dx, dx
		mov _cs, cs
		mov _ds, ds
		mov _ss, ss
		mov _es, es
		pushf
		pop _flags
	}
	regs.ax = _ax;
	regs.bx = _bx;
	regs.cx = _cx;
	regs.dx = _dx;
	regs.cs = _cs;
	regs.ds = _ds;
	regs.ss = _ss;
	regs.es = _es;
	regs.flags = _flags;
	return regs;
}

void set_regs(regs_t regs){
	short _ax = 0, _bx = 0, _cx = 0, _dx = 0, _flags = 0, _cs = 0,
		  _ds = 0, _ss = 0, _es = 0;
	_ax = regs.ax;
	_bx = regs.bx;
	_cx = regs.cx;
	_dx = regs.dx;
	_cs = regs.cs;
	_ds = regs.ds;
	_ss = regs.ss;
	_es = regs.es;
	_flags = regs.flags;
	_asm{
		mov ax, _ax
		mov bx, _bx
		mov cx, _cx
		mov dx, _dx
		mov cs, _cs
		mov ds, _ds
		mov ss, _ss
		mov es, _es
		push _flags
		popf
	}
}
