[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
  pusha
  mov edx, VIDEO_MEMORY

print_string_pm_loop:
  mov al, [ebx]
  mov ah, WHITE_ON_BLACK

  cmp al, 0 ;end of string at 0
  je print_string_pm_done

  mov [edx], ax

  add ebx, 1 ; next string char
  add edx, 2 ; next vid cell

  jmp print_string_pm_loop ;start over

print_string_pm_done:
  popa
  ret
