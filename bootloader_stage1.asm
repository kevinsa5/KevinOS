;==================================
; KevinOS bootloader 2
; Written March 28, 2014
;==================================

	ORG 0x7C00
    jmp 0x0000:start

; Start execution

start:

	;xor ax, ax
    
    mov ax, 0
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

; Detect Low Memory (below 1Mb, usually returns < 640Kb)

    xor ax, ax
    int 0x12
    jc .mem_error
    jmp .mem_continue

.mem_error:
    jmp $
.mem_continue:
    ; ax now contains the # of kB from zero of continuous memory
    mov bx, ax
    call printint
    mov bx, MSG_KB
    call println

; Wait for keystroke
.wait:
	mov bx, MSG_2
	call println
	mov ah, 0
	int 0x16

; Load the second stage
; load AL sectors to ES:BX from drive DL
	mov bx, 0x7C00 + 512
	mov dl, [BOOT_DRIVE]
    
	push dx
	mov ah, 0x02		; 0x13 read sector

	mov al, 1		; how many sectors to read
	mov ch, 0		; cylinder 0
	mov dh, 0		; head 0
	mov cl, 2		; start at sector 2 (1 is boot sector)

	int 0x13
	
	jc .disk_error
	pop dx
	cmp al, 1
	jne .incomplete
	
	jmp .stage2

.disk_error:
	mov bx, ERROR1
	call println
	call halt
.incomplete:
	mov bx, ERROR2
	call println
	call halt

.stage2:
    mov bx, MSG_STAGE2
    call println
	mov bx, 0x07c0
    mov es, bx
    mov bx, 0x200
    push es
    push bx
    retf
	jmp $
	
	

; Functions-------------------------------

; string address in bx

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


BOOT_DRIVE		db 0

MSG_2			db 'Press a key to continue', 0
ERROR1          db 'Error1', 0
ERROR2          db 'Error2', 0
HALTING			db 'Halting', 0
NUM_BUFFER		db '000', 0
MSG_KB          db ' kb RAM', 0
MSG_STAGE2      db 'Jumping to second stage', 0

times 510-($-$$) db 0
dw 0xaa55


;====================================================================

