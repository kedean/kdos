#include "../kernel/system.h"
#include "../io/screen_io.h"
#include "../threads/threads.h"
#include "../drivers/screen.h"

void syscall_start_thread(interrupt_data_s* r, void* parameter){
    THREAD* thread = (THREAD*) parameter;
    thread->idata = *r;
}
void print_a();
void post_syscall_start_thread(void* parameter, int syscall_exit_flag){

    if(syscall_exit_flag == 0){ //if the syscall_exit_flag is 0, that means the threading system has spun up the thread
        THREAD* thread = (THREAD*) parameter;

        thread->func();
        thread_kill_current();
    }
}

/*
 * Support functions
 */

void syscall_handle_request(interrupt_data_s* r){
    void* parameter = (void*) r->eax;

    switch(r->ebx){ //the ebx value determines which syscall to use
        case SYS_START_THREAD:
            syscall_start_thread(r, parameter);
            break;
        default:
            printf("Unknown system call #%u", r->ebx);
            break;
    };

    r->ebx = SYSCALL_EXIT_FLAG;
}

void syscall_init(void){
    interrupts_register_callback(INT_SYSCALL, &syscall_handle_request);
}

void do_syscall(uint_t call_num, void* parameter){
    __asm__ __volatile__ ("mov %0, %%ebx" : : "r"(call_num) : "%ebx");
    __asm__ __volatile__ ("mov %0, %%eax" : : "r"(parameter) : "%eax");
    __asm__ __volatile__ ("int $0x30");

    int syscall_exit_flag; //this flag lets us determine if this is completing the original do_syscall exit, or the threading system spinning up a thread for the first time
    __asm__ __volatile__ ("mov %%ebx, %0" : "=r"(syscall_exit_flag));

    if(syscall_exit_flag == 0){
      int ebp;
      __asm__ __volatile__ ("mov %%ebp, %0" : "=r"(ebp));
      __asm__ __volatile__ ("mov %0, %%esp" : : "r"(ebp));
    }

    switch(call_num){
        case SYS_START_THREAD:
            post_syscall_start_thread(parameter, syscall_exit_flag);
            break;
    };
}
