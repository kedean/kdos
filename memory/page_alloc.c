#include "../kernel/system.h"

basic_lock allocator_lock;

int liballoc_lock(){
	lock(&allocator_lock);

	return 0;
}

int liballoc_unlock(){
	unlock(&allocator_lock);
	return 0;
}

void* liballoc_alloc(int pages){
	return heap_alloc_page_frames(pages);
}

void liballoc_free(void* start, int pages){
	heap_free_page_frames(start, pages);
}