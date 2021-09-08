gdt_start:
gdt_null: ; 1st 8 bytes shd be null, dd -> define double word i.e 4 bytes
    dd 0x0
    dd 0x0
; gdt_code & gdt_data are 8-byte structures
gdt_code: ; code segment descriptor
    ; base =0x0 , limit =0xfffff ,
    ; 1st flags: (present )1 (privilege )00 (descriptor type )1 -> 1001b
    ; type flags: (code )1 (conforming )0 (readable )1 (accessed )0 -> 1010b
    ; 2nd flags: (granularity )1 (32- bit default )1 (64- bit seg )0 (AVL )0 -> 1100b
    dw 0xffff ; limit (bits 0-15) dw -> 2 bytes
    dw 0x0 ; base (bits 0-15)
    db 0x0 ; base (bits 16-23) db (define-byte) -> 1 byte
    db 10011010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, limit (bits 16-19)
    db 0x0 ; base (bits 24-31)
gdt_data: ; data segement descriptor
    ; same except type flags 
    ; type flags: (code )0 (expand down )0 (writable )1 (accessed )0 -> 0010b
    dw 0xffff ; limit (bits 0-15) dw -> 2 bytes
    dw 0x0 ; base (bits 0-15)
    db 0x0 ; base (bits 16-23) db (define-byte) -> 1 byte
    db 10010010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, limit (bits 16-19)
    db 0x0 ; base (bits 24-31)
gdt_end: ; so that the assembler can calculate the size of the GDT for the GDT descriptor (below)
; GDT descriptior
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; always 1 less than true size, sizeof GDT
    dd gdt_start ; start address of GDT
; Define some handy constants for the GDT segment descriptor offsets, 
; which are what segment registors must conatin when in protecte mode. 
; For example, when we set DS = 0x10 in PM, the CPU knows that we mean it to use the segment
; described at offset 0x10 (i.e 16 bytes) in our GDT, which in our case is the DATA segment
; (0x0 -> NULL; 0x08 -> CODE; 0x10; Notice the 8 byte gap)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


