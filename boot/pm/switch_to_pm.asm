[bits 16]
; switch to protected mode
switch_to_pm:
    ; We must switch of interrupts until we have 
    ; setup the protected mode interrupt vector
    ; otherwise interrupts will run riot. 
    cli 
    lgdt [gdt_descriptor] ; load out gdt, which defines the protected code segements
    mov eax, cr0 ; set first bit of control register cr0 to switch to pm
    or eax, 0x1
    mov cr0, eax
    ; Make a far jump (i.e. to a new segment) to our 32-bit
    ; code. This also forces the CPU to flush its cache of
    ; pre-fetched and real-mode decoded instructions , which can
    ; cause problems.
    jmp CODE_SEG:init_pm 
[bits 32]
; Initialize registors and stack once in PM
init_pm:
    ; Now in PM, our old segments are meaningless, 
    ; so we point our segment registers to the data selector we defined in our GDT
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ;update our stack registor so that it is right on top of the free space
    mov ebp, 0x90000
    mov esp, ebp
    call BEGIN_PM
