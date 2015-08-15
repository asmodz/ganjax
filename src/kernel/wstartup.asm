%define STACK_SIZE 512

;=================CODESEG========================
SEGMENT _STARTUP CLASS=CODE 
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
mov sp, STACK_BEGIN+STACK_SIZE

;=================KERNELSEG======================
SEGMENT _TEXT CLASS=CODE 


;=================CONSTSEG=======================
SEGMENT CONST CLASS=DATA 


;=================CONST2SEG======================
SEGMENT CONST2 CLASS=DATA


;=================BSSEG==========================
SEGMENT _DATA CLASS=DATA 


;=================STACKSEG=======================
SEGMENT _STACK CLASS=STACK 
STACK_BEGIN: RESB STACK_SIZE

GROUP DGROUP _STACK _STARTUP _TEXT
