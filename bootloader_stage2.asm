;==================================
; KevinOS bootloader 2
; Written March 28, 2014
;==================================

	ORG 0x7E00
    jmp 0x0000:start

    ;BIOS_DATA_AREA equ 0x400 
	KERNEL_ADDRESS equ 0x200
	NUM_SECTORS equ 53

; GDT definitions
	gdt_start:
	gdt_null:
		dd 0x0
		dd 0x0
	gdt_code:
		dw 0xffff		; Limit (bits 0-15)
		dw 0x0			; Base (bits 0-15)
		db 0x0			; Base (bits 16-23)
		db 10011010b		; 1st flags, access byte
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
	mov bp, 0x7C00
	mov sp, bp
	mov ss, ax
	sti

; Store the boot drive for later:

	mov [BOOT_DRIVE], dl

; Detect Low Memory (below 1Mb, usually below 640Kb)

    xor ax, ax
    int 0x12
    jc .mem_error
    ; ax now contains the # of kB from zero of continuous memory
    mov bx, ax
    call printint
    mov bx, MSG_KB
    call println
    jmp .wait

.mem_error:
    jmp $

; Wait for keystroke
.wait:
	mov bx, MSG_2
	call println
	mov ah, 0
	int 0x16

; Load the kernel

	mov bx, MSG_READING
	call print
	mov bx, NUM_SECTORS
	call printint
	mov bx, MSG_FROMDISK
	call println

	mov bx, KERNEL_ADDRESS
	mov dh, NUM_SECTORS 
	mov dl, [BOOT_DRIVE]
	
; load DH sectors to ES:BX from drive DL
; Add a looping thing here to load one sector at a time, rather than all at once
	push dx
	mov ah, 0x02		; 0x13 read sector
;.load_loop:
	mov al, dh		; how many sectors to read
	mov ch, 0		; cylinder 0
	mov dh, 0		; head 0
	mov cl, 3		; start at sector 2 (1 is boot sector)
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
	mov bx, DISK_ERROR
	call println
	mov bx, MSG_READ
	call print
	mov bl, dh
	call printint
	mov bx, MSG_FROMDISK
	call println
	call halt



; Enter Protected Mode and jump to kernel

enter_pm:
	cli
	lgdt [gdt_descriptor]	; defines protected mode segments
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

    ; flush the prefetch input queue
    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor) 
    ; to load CS with proper PM32 descriptor)
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

	jmp KERNEL_ADDRESS
	jmp $
	
	

; Functions-------------------------------

; string address in bx
[BITS 16]
println:
	call print
	mov al, 0x0D		; LF CR
	int 0x10
	mov al, 0x0A
	int 0x10
	ret

print:
	mov ah, 0x0e		; int 0x10 teletype output
.print_repeat:	
	mov al, [bx]
	cmp al, 0
	je .print_done
	int 0x10
	inc bx
	jmp .print_repeat
.print_done:
	ret

halt:
	mov bx, HALTING
	call println
	jmp $

; int in bx, must be 000 - 999
printint:
	push dx
	push bx
	push ax
	xor dx, dx
	mov ax, bx
	mov bx, 100
	div bx		;divide dx:ax by bx, store in ax, remainder in dx
	mov [NUM_BUFFER], al
	add [NUM_BUFFER], byte '0'
	mov ax, dx
	xor dx, dx
	mov bx, 10
	div bx
	mov [NUM_BUFFER+1], al
	add [NUM_BUFFER+1], byte '0'
	mov [NUM_BUFFER+2], dx
	add [NUM_BUFFER+2], byte '0'
	mov bx, NUM_BUFFER
	call print
	pop ax
	pop bx
	pop dx
    ret

[BITS 32]
printpm:
	VIDEO_MEM equ 0xb8000
	WHITE_ON_BLACK equ 0x0f
	pusha
	mov edx, VIDEO_MEM
	add edx, 160*20
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
MSG_KB          db ' kb RAM', 0
MSG_2			db 'Press a key to load the kernel', 0
MSG_READ		db 'Read ', 0
MSG_READING		db 'Reading ', 0
MSG_FROMDISK	db ' sectors from disk', 0
MSG_4			db 'Done!', 0
MSG_5			db 'PM: Entering kernel', 0
DISK_ERROR		db 'Read failed!', 0
HALTING			db 'Halting', 0
NUM_BUFFER		db '000', 0

times 510-($-$$) db 0
dw 0xaa55
