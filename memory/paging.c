#include "../kernel/system.h"
#include "../io/screen_io.h"
extern void load_page_directory(uint32_t**);
extern void enable_paging();

uint32_t** _page_directory = 0;

uint32_t* get_page_table(){
	uint32_t* page_table = heap_alloc_page_frames(1);

	for(int i = 0; i < 1024; i++){
		page_table[i] = 0 | 2; //supervisor, write enabled, not present
	}

	return page_table;
}

uint32_t** get_page_directory(){
	uint32_t** page_directory = heap_alloc_page_frames(1);

	for(int i = 0; i < 1024; i++){
		page_directory[i] = (uint32_t*) ((uint32_t) get_page_table() | 2); //attributes: supervisor, write enabled, not present
	}

	return page_directory;
}

void paging_map_identity_page(void* addr, int size){
	uint32_t from = (uint32_t) addr;

	if(from % 4096 != 0){
		printf("Page-alignment error\n");
		halt();
	}

	if(_page_directory != 0){ //if not zero, we dont have a directory yet, fail silently

		uint32_t page_directory_index = (uint32_t) from >> 22;
		uint32_t page_table_index = (from >> 12) & 0x03ff;
		uint32_t* page_table = _page_directory[page_directory_index];
		if(((uint32_t)page_table & 0x1) == 0){ //page table not present, make one and insert it
			_page_directory[page_directory_index] = (uint32_t*) ((uint32_t)_page_directory[page_directory_index] | 3);

			paging_map_identity_page((void*) (((uint32_t) page_table) & 0xfffff000), 4096); //we need to be able to access the table after we enable paging
		}

		page_table = (uint32_t*) (((uint32_t) page_table) & 0xfffff000); //clear the flags
		page_table += page_table_index; //start from the desired index

		from = from & 0xfffff000; // discard bits we don't want
		int i = 0;
		for(;size>0;from+=4096,size-=4096,page_table++){
			i++;
			*page_table=from|3;     // mark page as supervisor, read/write, present.
		}
	}
}

void page_fault_handler(interrupt_data_s* r){
	uint32_t virtual_address;
	__asm__ __volatile__ ( "mov %%cr2, %0" : "=r"(virtual_address) ); //the virtual address that was attempted is stored in the cr2 register, extract it
	//uint32_t page_directory_index = (uint32_t) virtual_address >> 22;
	//uint32_t page_table_index = (virtual_address >> 12) & 0x03ff;

	printf("Page fault accesing %d from esp = %x\n", virtual_address, r->esp);
	int z = 0;
	int y = 3 / z;
	halt();
}

void paging_init(){
	interrupts_register_callback(INT_PAGE_FAULT, &page_fault_handler);

	uint32_t** page_directory = get_page_directory();

	_page_directory = page_directory;

	paging_map_identity_page(0x00000000, 1024 * 1024); //id page the first megabyte of space
	paging_map_identity_page(page_directory, PAGE_SIZE_BYTES); //id page the directory itself
	paging_map_identity_page(heap_get_metadata_ptr(), heap_get_metadata_size());

	load_page_directory(page_directory);
	enable_paging();
}
