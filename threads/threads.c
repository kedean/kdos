#include "../kernel/system.h"
#include "../kernel/ports.h"
#include "../memory/memory.h"
#include "../io/screen_io.h"
#include "../collections/linked_list.h"

/*
 * Timing functions
 */

uint_t milliseconds_left = 0;
double tick_speed = 0; //measured in milliseconds per tick
uint_t ticks_to_context_switch = 0;
uint_t context_switch_quantum = 0;
void thread_context_switch_handler(interrupt_data_s*);

list_s* active_threads = NULL;
list_node_s* current_thread_node = NULL;

void time_systimer_handler(interrupt_data_s* r){

	if(current_thread_node != NULL){
		list_node_s* examining = current_thread_node;

		do{
			THREAD* thread = (THREAD*) examining->data;

			if(thread != NULL && thread->sleep_millis_remaining > 0){
				thread->sleep_millis_remaining -= tick_speed;

				if(thread->sleep_millis_remaining < 0){ //TODO: use a max function here
					thread->sleep_millis_remaining = 0;
				}
			}
			examining = list_get_next(active_threads, examining);
		} while(examining != current_thread_node); //once it is the first thread again, we have looped
	}

	ticks_to_context_switch--;

	if(ticks_to_context_switch <= 0){
		ticks_to_context_switch = 0;

		if(active_threads != NULL){
			thread_context_switch_handler(r);
		}

		ticks_to_context_switch = context_switch_quantum;
	}
}

void time_init(unsigned short speed_hz){
	context_switch_quantum = 10; //in number of ticks
	ticks_to_context_switch = context_switch_quantum;

	interrupts_register_callback(INT_TIMER, &time_systimer_handler);

	tick_speed = 1000.0 / (double) speed_hz;

	uint_t divisor = 1193182 / speed_hz;
	if(!divisor){divisor++;}

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, divisor & 0xFF);
	port_byte_out(0x40, divisor >> 8);
}

void sleep(uint_t milliseconds){
	THREAD* thread = current_thread_node->data;
	thread->sleep_millis_remaining = (double) milliseconds;

	while(thread->sleep_millis_remaining > 0){
		thread_yield();
	}
}

/*
 * Threading functions
 */

extern uint_t get_eip(void);

void thread_init(){

	active_threads = list_create(sizeof(THREAD*));

	//create the thread struct for the kernel
	THREAD* kernel = (THREAD*) malloc(sizeof(THREAD));
	kernel->is_running = true;
	kernel->sleep_millis_remaining = 0;
	thread_queue(kernel);

	current_thread_node = active_threads->head;
}

THREAD* thread_create(void (*func)()){
	THREAD* thread = (THREAD*) malloc(sizeof(THREAD));
	thread->func = func;

	do_syscall(SYS_START_THREAD, thread);

	//allocate space for the thread to operate in
	char* thread_mem = (char*) malloc(8192); //get a page of memory for the process
	thread->idata.esp = (uint_t) thread_mem + (8192 - 64);
	thread->idata.ebp = thread->idata.esp;

	//initialize the threads memory space with initial parameters to its function (do_syscall)
	*((int*)thread->idata.ebp + 2) = SYS_START_THREAD;
	*((int*)thread->idata.ebp + 3) = thread;

	//initialize the rest of the thread
	thread->is_running = false;
	thread->sleep_millis_remaining = 0;

	return thread;
}

void thread_queue(THREAD* thread){
	list_append(active_threads, thread);
}

void thread_yield(){
	ticks_to_context_switch = 0;
}

void thread_kill_current(){
	/*interrupts_disable();
	THREAD* thread = running_threads[current_thread_index];
	int next_thread_index = get_future_thread_index(current_thread_index);
	running_threads[current_thread_index] = NULL;
	free(thread);
	milliseconds_to_context_switch = 0;
	interrupts_enable();

	for(;;);*/
}

void thread_context_switch_handler(interrupt_data_s* r){
	list_node_s* next_thread_node = list_get_next(active_threads, current_thread_node);

	if(next_thread_node != NULL){
		THREAD* future_thread = next_thread_node->data;
		THREAD* current_thread = current_thread_node->data;

		current_thread->idata = *r;
//		printf("ni = %d, eax = %x, ebp = %x, esp = %x\n", next_thread_index, future_thread->idata.eax, future_thread->idata.ebp, future_thread->idata.esp);

		current_thread->is_running = false;
		future_thread->is_running = true;

		current_thread_node = next_thread_node;
		*r = future_thread->idata;

		// printf("\n-- context switch --\n");
	}
}
