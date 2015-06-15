#include "../kernel/system.h"
#include "../synch/synch.h"

void lock(basic_lock* l){
	bool result;

	do{
		result = __sync_bool_compare_and_swap(l, 0, 1);
	} while(result == false);
}

void unlock(basic_lock* l){
	__sync_bool_compare_and_swap(l, 1, 0);
}