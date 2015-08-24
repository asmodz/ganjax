; Yay, this work on toy-os :D :D
mov al, byte [x]
mov ah, 0xe
int 0x10

mov cx, 10
mov bx, '0'
l:
	mov ah, 0xe
	mov al, 'c'
	inc bx
	int 0x1
	loop l
	
mov ax, 6
ret
segment _DATA 
x: db "Siema", 0x0
