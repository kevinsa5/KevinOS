; boot sector for a (C) kernel in 32-bit PM
[bits 16]
org 0x7c00

jmp start
;  nop
;  db '        '
;  dw 512                        ; bytes per sector
;  db 1                          ; sectors per cluster
;  dw 36                         ; reserved sector count
;  db 2                          ; number of FATs
;  dw 16*14                      ; root directory entries
;  dw 18*2*80                    ; sector count
;  db 0F0h                       ; media byte
;  dw 9                          ; sectors per fat
;  dw 18                         ; sectors per track
;  dw 2                          ; number of heads
;  dd 0                          ; hidden sector count
;  dd 0                          ; number of sectors huge
;  db 0                          ; drive number
;  db 0                          ; reserved
; db 29h                        ; signature
;  dd 0                          ; volume ID
;  db '           '              ; volume label
;  db 'ASDFG   '                 ; file system type
  
BOOT_DRIVE 		db 0
MSG_1 			db 'Bootloader running successfully!', 0
MSG_2 			db 'Press a key to load kernel and enter PM', 0
MSG_3 			db 'Entering the kernel', 0
DISK_ERROR_MSG 	db 'Disk Read failed!', 0 
INCOMPLETE_READ_MSG db 'Wrong number of sectors read', 0
HALTING_MSG		db 'Execution Halted', 0

KERNEL_OFFSET equ 0x200

start:

xor ax, ax
mov ds, ax
mov es, ax
cli
mov bp, 0x9000			; Set up the stack
mov sp, bp
mov ss, ax
sti
mov[BOOT_DRIVE], dl		; the BIOS stores boot drive in DL

mov bx, MSG_1
call println
mov bx, MSG_2
call println
mov ah, 0
int 16h					; wait for keystroke

call load_kernel
call enter_pm
jmp $

println:
	call print
	mov al, 0Dh 		; LF CR
	int 0x10
	mov al, 0Ah
	int 0x10
	ret

print:
	mov ah, 0x0e
.repeat:
	mov al, [bx]
	cmp al, 0
	je .done
	int 0x10
	inc bx
	jmp .repeat
.done:
	ret

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh, 56;Change as kernel increases in size
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret
	
; load DH sectors to ES:BX from drive DL
disk_load:
	push dx
	mov ah, 0x02		; BIOS read sector
	mov al, dh
	mov ch, 0			; cylinder 0
	mov dh, 0			; head 0
	mov cl, 2			; second sector (first is boot sector)
	int 0x13
	
	jc .disk_error
	
	pop dx
	cmp dh, al
	jne .incomplete
	ret
.disk_error:
	mov bx, DISK_ERROR_MSG
	call println
	mov bx, HALTING_MSG
	call println
	;mov al, 0Ah
	;int 0x10
	jmp $
.incomplete:
	mov bx, INCOMPLETE_READ_MSG
	call println
	mov bx, HALTING_MSG
	call print
	jmp $

; GDT definitions
gdt_start:
gdt_null:
	dd 0x0
	dd 0x0
gdt_code:
	dw 0xffff		; Limit (bits 0-15)
	dw 0x0			; Base (bits 0-15)
	db 0x0			; Base (bits 16-23)
	db 10011010b	; 1st flags, type flags
	db 11001111b	; 2nd flags, Limit (bits 16-19)
	db 0x0			; Base (bits 24-31)
gdt_data:
	dw 0xffff		; Limit (bits 0-15)
	dw 0x0			; Base (bits 0-15)
	db 0x0			; Base (bits 16-23)
	db 10010010b	; 1st flags, type flags
	db 11001111b	; 2nd flags, Limit (bits 16-19)
	db 0x0			; Base (bits 24-31)
gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
	
[bits 32]
VIDEO_MEM equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_pm:
	pusha
	mov edx, VIDEO_MEM
.repeat:
	mov al, [ebx]
	mov ah, WHITE_ON_BLACK
	cmp al, 0
	je .done
	mov [edx], ax
	add ebx, 1
	add edx, 2		; 2 bytes in video memory (1 for ascii, 1 for settings)
	jmp .repeat
.done:
	popa
	ret
	
[bits 16]
enter_pm:
	cli						; turn off interrupts
	lgdt [gdt_descriptor]	; defines protected mode segments
	
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp CODE_SEG:init_pm	; forces the CPU to flush real-mode commands

[bits 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000
	mov esp, ebp
	
	call begin_pm
	
[bits 32]
begin_pm:
	mov ebx, MSG_3
	call print_pm
	call KERNEL_OFFSET		; jump to the asm entry routine
	jmp $
	
times 510-($-$$) db 0
dw 0xaa55
