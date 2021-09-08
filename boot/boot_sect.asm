; Useful reference 
; -> https://stackoverflow.com/questions/34216893/disk-read-error-while-loading-sectors-into-memory/34382681
; -> https://stackoverflow.com/questions/26645676/xor-ax-ax-when-loading-segment-register
; A boot sector that enters 32-bit protected mode.
; Boot sector needs to fit in 512 bytes
[bits 16]
[org 0x7C00] ; BIOS dumps us here
KERNEL_OFFSET equ 0x1000 ; this is the offset the kernal is loaded

xor ax, ax
mov ds, ax
mov es, ax  
mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in dl, best to remember for later 
mov ax, 0x07E0
cli ; clear interrupts
mov ss, ax 
mov sp, 0x1200 ; SS:SP
sti ; set interrupt flag
mov si, MSG_REAL_MODE       
call print_string ; booting from 16-bit real mode 
call load_kernel
call switch_to_pm ; Note that, we never return from here
jmp $

%include "real_mode/print_string.asm"
%include "disk/disk_load.asm"
%include "pm/gdt.asm"
%include "pm/print_string_pm.asm"
%include "pm/switch_to_pm.asm"

[bits 16]
load_kernel: 
    mov si, MSG_LOAD_KERNEL
    call print_string
    mov bx, KERNEL_OFFSET       
    mov dh, 15       
    mov dl, [BOOT_DRIVE]                      
    call disk_load 
    mov si, MSG_LOAD_KERNEL_COMPLETE
    call print_string                                                 
    ret
[bits 32]
; This is where we arrive after switching and initialising protected mode
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; Now jump to the address of our loaded 
                       ; kernel code , assume the brace position, 
                       ; and cross your fingers. Here we go! 
    jmp $ ; Hang
; Global variables
BOOT_DRIVE db 0
SECTORS db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode. ", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode. ", 0 
MSG_LOAD_KERNEL db "Loading kernel into memory.                        ", 0
MSG_LOAD_KERNEL_COMPLETE db "Completed loading kernel into memory. ", 0

times 510-($-$$) db 0 ; write 0s till it's the 510th byte
dw 0xAA55 ; last 2 bytes - magic number

; 15 sector padding
;times 15*256 dw 0xDADA
