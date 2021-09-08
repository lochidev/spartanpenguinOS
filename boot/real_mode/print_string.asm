print_string:
  push ax
  push bx
  push si
  mov bx, 0x0007  ;BL=WhiteOnBlack BH=Display page 0
  mov ah, 0x0E    ;Teletype function
 print_loop:
  mov al, [si]
  cmp al, 0
  je return
  int 0x10
  inc si
  jmp print_loop
 return:
  pop si
  pop bx
  pop ax
  ret