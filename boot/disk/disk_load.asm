disk_load:
    mov [SECTORS], dh
    mov ch, 0x00 ; Select cylinder 0 
    mov dh, 0x00 ; Select head 0
    mov cl, 0x02 ; Start reading from second sector (i.e.  after the boot sector)
next_group:
    mov di, 5 ; Max 5 tries
again:
    mov ah, 0x02 ;BIOS Read sectors function
    mov al, [SECTORS]
    int 0x13 ; BIOS interrupt 
    jc maybe_retry ; Jump if error (i.e. carry flag set)
    sub [SECTORS], al ;Remaining sectors
    jz ready
    mov cl, 0x01      ; Always sector 1
    xor dh, 1         ; Next head on diskette!
    jnz next_group
    inc ch            ; Next cylinder
    jmp next_group
maybe_retry:
    mov ah, 0x00      ;Reset diskdrive
    int 0x13
    dec di
    jnz again
    jmp disk_error
ready:  
    ret
disk_error:
    mov si, DISK_ERROR_MSG
    call print_string
    jmp $
; Variables
DISK_ERROR_MSG db "Disk read error! ", 0