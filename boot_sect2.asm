;==================================
; KevinOS bootloader 2
; Written March 28, 2014
;==================================

	ORG 0x7C00
	jmp 0x0000:start

	KERNEL_ADDRESS equ 0x200
	NUM_SECTORS equ 57

; GDT definitions
	gdt_start:
	gdt_null:
		dd 0x0
		dd 0x0
	gdt_code:
		dw 0xffff		; Limit (bits 0-15)
		dw 0x0			; Base (bits 0-15)
		db 0x0			; Base (bits 16-23)
		db 10011010b		; 1st flags, type flags
		db 11001111b		; 2nd flags, Limit (bits 16-19)
		db 0x0			; Base (bits 24-31)
	gdt_data:
		dw 0xffff		; Limit (bits 0-15)
		dw 0x0			; Base (bits 0-15)
		db 0x0			; Base (bits 16-23)
		db 10010010b		; 1st flags, type flags
		db 11001111b		; 2nd flags, Limit (bits 16-19)
		db 0x0			; Base (bits 24-31)
	gdt_end:
	gdt_descriptor:
		dw gdt_end - gdt_start - 1
		dd gdt_start
	CODE_SEG equ gdt_code - gdt_start
	DATA_SEG equ gdt_data - gdt_start
	
; Start execution

	start:

	xor ax, ax
	mov ds, ax
	mov es, ax

; Set up the stack:

	cli
	mov bp, 0x9000
	mov sp, bp
	mov ss, ax
	sti

; Store the boot drive for later:

	mov [BOOT_DRIVE], dl

; Print out messages

	mov bx, MSG_1
	call println
	mov bx, MSG_2
	call println

; Wait for keystroke

	mov ah, 0
	int 0x16

; Load the kernel

	mov bx, MSG_3
	call println

	mov bx, KERNEL_ADDRESS
	mov dh, NUM_SECTORS 
	mov dl, [BOOT_DRIVE]
	
; load DH sectors to ES:BX from drive DL

	push dx
	mov ah, 0x02		; 0x13 read sector
	mov al, dh
	mov ch, 0		; cylinder 0
	mov dh, 0		; head 0
	mov cl, 2		; start at sector 2 (1 is boot sector)
	int 0x13
	jc .disk_error
	pop dx
	cmp dh, al
	jne .incomplete
	
	mov bx, MSG_4
	call println
	jmp enter_pm

.disk_error:
	mov bx, DISK_ERROR
	call println
	call halt
.incomplete:
	mov bx, INCOMPLETE_READ
	call println
	call halt



; Enter Protected Mode and jump to kernel

enter_pm:
	cli
	lgdt [gdt_descriptor]	; defines protected mode segments
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:init_pm	; flushes real-mode commands

[BITS 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ebp, 0x90000
	mov esp, ebp
	mov ebx, MSG_5
	call printpm
	call KERNEL_ADDRESS
	jmp $
	
	

; Functions-------------------------------

; string address in bx
[BITS 16]
println:
	mov ah, 0x0e		; int 0x10 teletype output
.println_repeat:	
	mov al, [bx]
	cmp al, 0
	je .println_done
	int 0x10
	inc bx
	jmp .println_repeat
.println_done:
	mov al, 0x0D		; LF CR
	int 0x10
	mov al, 0x0A
	int 0x10
	ret

halt:
	mov bx, HALTING
	call println
	jmp $

[BITS 32]
printpm:
	VIDEO_MEM equ 0xb8000
	WHITE_ON_BLACK equ 0x0f
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


[BITS 16]


BOOT_DRIVE		db 0
MSG_1			db 'Bootloader is successfully running.', 0
MSG_2			db 'Press a key to load and enter the kernel.', 0
MSG_3			db 'Attempting to read the disk...', 0
MSG_4			db 'Done!', 0
MSG_5			db 'Entering the kernel...', 0
DISK_ERROR		db 'The disk read failed!', 0
INCOMPLETE_READ		db 'The wrong number of sectors were read!', 0
HALTING			db 'Halting execution...', 0

times 510-($-$$) db 0
dw 0xaa55
