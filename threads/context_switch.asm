[bits 32]
[global context_switch]
[global get_eip]

get_eip:
	mov eax, [esp]
	ret

context_switch:
	;struct passed in as arg:
	;unsigned int gs, fs, es, ds;      /* pushed the segs last */
    ;unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    ;unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    ;unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */

	iret
