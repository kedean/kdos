; Entry point for the kernel. This makes the actual location of the main function irrelavent

[bits 32]
[extern main]
call main
jmp $