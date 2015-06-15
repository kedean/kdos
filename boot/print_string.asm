;
; 16-bit real mode printing functions, using BIOS routines
;

[bits 16]

print_string:
	pusha
	mov ah, 0x0e
	print_loop:
	mov al, [bx]
	cmp al, 0
	je print_string_done
	int 0x10
	add bx, 0x1
	jmp print_loop
	print_string_done:
	popa
	ret
 
print_hex:
	pusha

	mov bx, dx
	shr bx, 12 ;move the needed byte to the lowest 4 bits
	and bx, 0x0000000f ;zero out the upper 12 bits
	cmp bl, 0xa ;if its below 10, add the 4 lower bits to the current spot of HEX_OUT
	jl add_hex_val1
	add bx, 0x27 ;if its above nine, add 39 to the number for ascii conversion
	add_hex_val1:
	add [HEX_OUT + 2], bl ; add the new value to the spot in HEX_OUT, which should be the char '0'

	mov bx, dx
	shr bx, 8
	and bx, 0x0000000f
	cmp bl, 0xa
	jl add_hex_val2
	add bx, 0x27
	add_hex_val2:
	add [HEX_OUT + 3], bl

	mov bx, dx
	shr bx, 4
	and bx, 0x0000000f
	cmp bl, 0xa
	jl add_hex_val3
	add bx, 0x27
	add_hex_val3:
	add [HEX_OUT + 4], bl

	mov bx, dx
	and bx, 0x0000000f
	cmp bl, 0xa
	jl add_hex_val4
	add bx, 0x27
	add_hex_val4:
	add [HEX_OUT + 5], bl
	

	mov bx, HEX_OUT
	call print_string

	mov byte [HEX_OUT + 2], '0' ;zero out the old spots
	mov byte [HEX_OUT + 3], '0'
	mov byte [HEX_OUT + 4], '0'
	mov byte [HEX_OUT + 5], '0'

	popa
	ret
 	
HEX_OUT: db '0x0000', 0