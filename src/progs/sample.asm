; Yay, this work on toy-os :D :D
mov cx, 10
mov bx, '0'
l:
	mov ah, 0xe
	mov al, bl
	inc bx
	int 0x10
	loop l
	
mov ax, 6
ret
