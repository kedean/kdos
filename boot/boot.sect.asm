;
; A boot sector that enters 32-bit protected mode
;

[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ;the kernel is linked to this address, so that is where it starts

mov [BOOT_DRIVE], dl ;boot drive is stored in this register, may need it later

mov bp, 0x9000 ;set the stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel

mov bx, MSG_LOAD_KERNEL_SUCCESS
call print_string

call switch_to_protected
jmp $ ;loop forever

%include "boot/gdt.asm"
%include "boot/protected_mode.asm"
%include "boot/print_string.asm"
%include "boot/print_string_pm.asm"
%include "boot/disk_load.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET ;disk loading params get set up
	mov dh, 52 ;fifteen sectors
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
BEGIN_PM: ;where we start after switching modes
  mov ebx, MSG_PROT_MODE
  call print_string_pm
  call KERNEL_OFFSET
  jmp $;

MSG_REAL_MODE:
  db "In 16-bit real mode", 0
MSG_PROT_MODE:
  db "In 32-bit protected mode", 0
BOOT_DRIVE:
	db 0
MSG_LOAD_KERNEL:
	db "Ima loadin' mah kernel!", 0
MSG_LOAD_KERNEL_SUCCESS:
	db "KERNEL LOADED", 0

;bootsector padding
times 510-($-$$) db 0
dw 0xaa55 ;magic number to start the os
