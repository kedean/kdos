#include "../drivers/keyboard.h"
#include "../kernel/system.h"

#include "../programs/echo.h"

#include "../io/screen_io.h"

void print_a(){
	while(1){
		sleep(2500);
		printf("\nthread a\n");
		sleep(2500);
	}
}

void print_b(){
	while(1){
		sleep(5000);
		printf("\nthread b\n");
		thread_yield();
	}
}

/*
 * Effective system entry point
 */
void main(){
	interrupts_disable();
	clear_screen();
	interrupts_init();
	syscall_init();
	time_init(50);
	heap_init_page_frame_heap((void*) 0x100000, 0xfffff);
	paging_init();
	keyboard_init();

	interrupts_enable();
	thread_init(1000);

	THREAD* a = thread_create(&print_a);
	thread_queue(a);

	THREAD* b = thread_create(&print_b);
	thread_queue(b);

	echo();

	halt();
}
