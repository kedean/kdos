[bits 32]

global load_page_directory

load_page_directory:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret

global enable_paging

enable_paging:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret