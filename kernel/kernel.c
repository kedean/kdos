#include "../drivers/keyboard.h"
#include "../kernel/system.h"

#include "../programs/programs.h"

#include "../io/screen_io.h"

/*
 * Effective system entry point
 */
void main(){
	clear_screen();

	interrupts_disable();
	interrupts_init();
	syscall_init();
	time_init(1000); //a 1000hz internal clock cycle, a clock tick every millisecond
	heap_init_page_frame_heap((void*) 0x100000, 0xfffff);
	paging_init();
	keyboard_init();
	pipe_init(100);

	screen_init();

	interrupts_enable();
	thread_init(1000);

	THREAD* prog = thread_create(&clock);
	thread_queue(prog);

	halt();
}
