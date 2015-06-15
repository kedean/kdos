#pragma once

#include "../threads/threads.h"

enum syscalls {
    SYS_START_THREAD
};

#define SYSCALL_EXIT_FLAG 0x10; //if the ebx register equals this after int 0x30, then one can assume the syscall routine just ended. Otherwise, another mechanism jumped the processor to that location.

void syscall_init();
void do_syscall(uint_t call_num, void* parameter);
