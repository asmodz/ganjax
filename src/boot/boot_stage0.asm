bits    16                      
org 	0x0000
start: jmp _boot_setup_segments  

;==============BIOS_PARAMETER_BLOCK================ 
bpb_oem					 DB "Ganjax  "			; OEM identifier (Cannot exceed 8 bytes!)
bpb_bytes_per_sector:  	 DW 512					; Bajty na 1 sektor
bpb_sectors_per_cluster: DB 1					; Sektory na klaster
bpb_reserved_sectors: 	 DW 1					; Bootloader.
bpb_number_of_fats:		 DB 2					; Zawsze 2 dla FAT12
bpb_root_entries: 		 DW 224					; Maksymalnie 224 pliki
bpb_total_sectors: 		 DW 2880				; Liczba sektorów na nośniku
bpb_media: 				 DB 0xf8  				; 0xF8
bpb_sectors_per_fat:	 DW 9					; 8 || 9
bpb_sectors_per_track: 	 DW 18					; ilość sektorów na jeden track
bpb_heads_per_cylinder:	 DW 2					; heady na cylinder
bpb_hidden_sectors: 	 DD 0					; 0
bpb_total_sectors_big:   DD 0					; 0

bs_drive_number: 	     DB 0					; Numer dysku tj. 0x00
bs_unused: 				 DB 0					; Ilość nieużywanych
bs_ext_boot_signature: 	 DB 0x29				; Typ i wersja BPB 0x29/x28 tj MS/PC-DOS 4.0v
bs_serial_number:	     DD 0xB16B00B5			; BigBoobs serial
bs_volume_label: 	     DB "GANJAX DISK"		; Musi mieć dokładnie 11 bajtów
bs_file_system:	         DB "FAT12   "			; Musi mieć 8 dokładnie bajtów 
;==================================================

_boot_print_string:
	lodsb              
    or  al, al          
    jz  _boot_print_done       
    mov ah, 0eh        
    int 10h
    jmp _boot_print_string 
             
    _boot_print_done:
		ret                 
; Load to ES:BX
_boot_read_sectors:
	 ._boot_main_read_sectors:
		mov     di, 0x0005 
		                         
     ._boot_sector_loop:
          push    ax
          push    bx
          push    cx
          call    _boot_lba_to_chs                              
          mov     ah, 0x02                            
          mov     al, 0x01                            
          mov     ch, BYTE [buff_track]            
          mov     cl, BYTE [buff_sector]          
          mov     dh, BYTE [buff_head]             
          mov     dl, BYTE [bs_drive_number]          
          int     0x13                               
          jnc     ._boot_reading_sectors_ok                            
          xor     ax, ax                              
          int     0x13                                
          dec     di                                 
          pop     cx
          pop     bx
          pop     ax
          jnz     ._boot_sector_loop   
                               
     ._boot_reading_sectors_ok:
          pop     cx
          pop     bx
          pop     ax
          add     bx, WORD [bpb_bytes_per_sector]      
          inc     ax                                 
          loop    ._boot_main_read_sectors                              
          ret

_boot_cluster_to_lba:
	sub     ax, 0x0002                         
    xor     cx, cx
    mov     cl, BYTE [bpb_sectors_per_cluster]     
    mul     cx
    add     ax, WORD [data_sector]               
    ret
     
_boot_lba_to_chs:
    xor     dx, dx                              
    div     WORD [bpb_sectors_per_track]           
    inc     dl                                  
    mov     BYTE [buff_sector], dl
    xor     dx, dx                            
    div     WORD [bpb_heads_per_cylinder]          
    mov     BYTE [buff_head], dl
    mov     BYTE [buff_track], al
    ret

_boot_compute_root_size:
	xor cx, cx
	xor dx, dx
	mov ax, 32
	mul WORD [bpb_root_entries]
	div WORD [bpb_bytes_per_sector]
	xchg ax, cx
	ret

_boot_compute_root_position:
	mov al, BYTE [bpb_number_of_fats]
	mul WORD [bpb_sectors_per_fat]
	add ax, WORD [bpb_reserved_sectors]
	mov WORD [data_sector], ax
	add WORD [data_sector], cx
	ret

; DS, FS, GS = 0x07c0
; ES 		 = 0x0050
; SS 		 = 0x0000
; SP 		 = 0xFFFF
_boot_setup_segments:
	cli
	mov ax, 0x07c0
	mov ds, ax
	mov fs, ax
	mov gs, ax
	
	mov ax, 0x0050
	mov es, ax
		
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFF
	sti
	
main:
	call _boot_compute_root_size
	call _boot_compute_root_position
	
	; Load entries to 0x0050:0x0000 (0x00000500)
	mov bx, 0x0000
	call _boot_read_sectors		
		
	; Search image
	mov cx, WORD [bpb_root_entries]
	mov di, 0x000
	
	._boot_find_image_loop:
		push cx
		mov cx, 11				; Entry have 11 chars
		mov si, image_name
		push di
		rep cmpsb				; Compare name
		pop di
		je _boot_image_found
		pop cx
		add di, 32				; Next entry
		loop ._boot_find_image_loop
		jmp _boot_image_not_found

	
	_boot_image_found:
		mov dx, WORD [di + 0x001A]	; Get starring cluster (0x1a = cluster offst)
		mov WORD [cluster], dx		
		xor ax, ax
		mov al, BYTE [bpb_number_of_fats]
		mul WORD [bpb_sectors_per_fat]
		mov cx, ax
	
	; Read FAT into 0x0050 : 0x0000 = 0x00000500 
	; 0x00000500 is 30 KB free memory
	
	;============TESTING============================
	; Load fat to 0x07e0
	mov ax, 0x07c0
	mov es, ax
	mov bx, 0x0200
	mov ax, WORD [bpb_reserved_sectors]
	;===============================================
	
	;mov ax, WORD [bpb_reserved_sectors]
	;mov bx, 0x0000
	call _boot_read_sectors
	

	;==========TESTING=================================
	; Load kernel to 0x00000500
	mov ax, 0x0050
	mov es, ax
	mov bx, 0x0000
	; FAT is in 0x7e00
	mov ax, 0x07e0
	mov gs, ax
	;==================================================
	;mov ax, 0x07c0
	;mov es, ax
	
	;mov bx, 0x0200
	;mov ax, 0x0050
	;mov gs, ax
	
	push bx
	
	_boot_load_image:
		mov ax, WORD [cluster]
		pop bx
		call _boot_cluster_to_lba
		xor cx, cx
		mov cl, BYTE [bpb_sectors_per_cluster]
		call _boot_read_sectors
		push bx
		mov ax, WORD [cluster]
		mov cx, ax
		mov dx, ax
		shr dx, 0x0001
		add cx, dx
		mov bx, 0x0000
		add bx, cx
		mov dx, WORD [gs:bx]
		test ax, 0x0001
		jnz ._boot_odd_cluster
		
	._boot_even_cluster:
		and dx, 0000111111111111b
		jmp ._boot_loading_done
		
	._boot_odd_cluster:
		shr dx, 0x0004
		
	._boot_loading_done:
		mov WORD [cluster], dx
		cmp dx, 0x0FF0
		jb _boot_load_image
			
	mov si, msg_ok
	call _boot_print_string
	;jmp 0x07c0:0x0200
	
	jmp 0x0050:0x0000
	
	
_boot_image_not_found:
	mov si, msg_failure
	call _boot_print_string
	mov ah, 0x00
	int 0x16
	int 0x19
          
buff_sector DB 0x00
buff_head   DB 0x00
buff_track  DB 0x00
     
data_sector DW 0x0000
cluster     DW 0x0000
image_name  DB "KERNEL  SYS", 0x00

msg_ok      DB 0x0D, 0x0A, "KERNEL LOAD OK.", 0x0d, 0x0a, "JMP TO 0x0050:0x0000...", 0x0D, 0x0A, 0x00
msg_failure DB 0x0D, 0x0A, "KERNEL LOAD ERR.", 0x0A, 0x00
     
TIMES 510-($-$$) DB 0
DW 0xAA55         
