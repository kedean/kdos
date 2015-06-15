#include "../kernel/system.h"
#include "../kernel/ports.h"
#include "../memory/memory.h"
#include "../io/screen_io.h"

/*
 * Timing functions
 */

uint_t milliseconds_left = 0;
uint_t tick_speed;
uint_t milliseconds_to_context_switch;
uint_t context_switch_quantum;
void thread_context_switch_handler(interrupt_data_s*);

THREAD** running_threads; //implemented as a simple array of threads, non-zero entries are running threads, NOTE: replace this with a more efficient structure
uint_t max_thread_count;
uint_t current_thread_index;

void time_systimer_handler(interrupt_data_s* r){
	for(int i = 0; i < max_thread_count; i++){
		if(running_threads[i] != NULL && running_threads[i]->sleep_millis_remaining > 0){
			running_threads[i]->sleep_millis_remaining -= tick_speed;
			if(running_threads[i]->sleep_millis_remaining < 0){
				running_threads[i]->sleep_millis_remaining = 0;
			}
		}
	}

	if(tick_speed > milliseconds_to_context_switch){
		milliseconds_to_context_switch = 0;

		thread_context_switch_handler(r);

		milliseconds_to_context_switch = context_switch_quantum;
	} else{
		milliseconds_to_context_switch -= tick_speed;
	}
}

void time_init(unsigned short speed){
	context_switch_quantum = 100;
	milliseconds_to_context_switch = context_switch_quantum;

	interrupts_register_callback(INT_TIMER, &time_systimer_handler);

	tick_speed = speed;

	unsigned int divisor = 1193180 / speed;
	if(!divisor){divisor++;}

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, divisor & 0xFF);
	port_byte_out(0x40, divisor >> 8);
}

void sleep(int milliseconds){
	THREAD* thread = running_threads[current_thread_index];
	thread->sleep_millis_remaining = milliseconds;
	while(thread->sleep_millis_remaining > 0){
		thread_yield();
	}
}

/*
 * Threading functions
 */

extern uint_t get_eip(void);

void thread_init(uint_t max_threads){
	max_thread_count = max_threads;

	uint_t thread_heap_size = sizeof(THREAD*) * max_threads;
	running_threads = malloc(thread_heap_size);
	memset(running_threads, 0, thread_heap_size);

	//create the thread struct for the kernel
	THREAD* kernel = (THREAD*) malloc(sizeof(THREAD));
	kernel->is_running = true;
	kernel->sleep_millis_remaining = 0;
	current_thread_index = thread_queue(kernel);
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

int thread_queue(THREAD* thread){
	for(int i = 0; i < max_thread_count; i++){
		if(running_threads[i] == 0){ //free spot
			running_threads[i] = thread;
			return i;
		}
	}

	return -1;
}

void thread_yield(){
	milliseconds_to_context_switch = 0;
}

void thread_kill_current(){
	interrupts_disable();
	THREAD* thread = running_threads[current_thread_index];
	int next_thread_index = get_future_thread_index(current_thread_index);
	running_threads[current_thread_index] = NULL;
	free(thread);
	milliseconds_to_context_switch = 0;
	interrupts_enable();

	for(;;);
}

int get_future_thread_index(uint_t current){
	uint_t next_index = current;
	for(;;){
		next_index++;

		if(next_index == current){
			return -1;
		} else if(next_index == max_thread_count){
			next_index = -1; //-1 means it will increment to 0 next round, resetting the queue
		} else if(running_threads[next_index] != NULL){
			break;
		}
	}

	return next_index;
}

void thread_context_switch_handler(interrupt_data_s* r){
	THREAD* current_thread = running_threads[current_thread_index];
	int next_thread_index = get_future_thread_index(current_thread_index);

	//store registers
	if(next_thread_index >= 0){
		current_thread->idata = *r;
		THREAD* future_thread = running_threads[next_thread_index];
//		printf("ni = %d, eax = %x, ebp = %x, esp = %x\n", next_thread_index, future_thread->idata.eax, future_thread->idata.ebp, future_thread->idata.esp);

		current_thread->is_running = false;
		future_thread->is_running = true;

		current_thread_index = next_thread_index;
		*r = future_thread->idata;

		// printf("\n-- context switch --\n");
	}
}
