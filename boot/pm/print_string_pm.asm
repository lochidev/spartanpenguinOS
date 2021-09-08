[bits 32]
; Constants
VIDEO_MEMORY equ 0xb8000 ; start of vid memory
WHITE_ON_BLUE equ 0x1f
print_string_pm:
    pusha 
    mov edx, VIDEO_MEMORY 
    jmp print_string_pm_loop
print_string_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLUE
    cmp al, 0
    je print_string_pm_done
    mov [edx], ax ; move character and attributes to current cell
    add ebx, 1 ; next char pls
    add edx, 2 ; next character cell
    jmp print_string_pm_loop
print_string_pm_done:
    popa
    ret

