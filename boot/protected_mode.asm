[bits 16]
;switches to protected mode
switch_to_protected:
  cli ;disable interrupts
  lgdt [gdt_descriptor] ;pass the gdt to the cpu

  mov eax, cr0 ;set the first bit of cr0 to 1 to switch to protected mode
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:init_pm

[bits 32]
init_pm:
  mov ax, DATA_SEG ;old segments dont do anything
  mov ds, ax ;redirect segments to
  mov ss, ax ;the spots defined in GDT
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000 ;put the stack at the top
  mov esp, ebp ;of the free space

  call BEGIN_PM
