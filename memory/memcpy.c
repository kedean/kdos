#include "../memory/memory.h"

void memcpy(void* vsource, void* vdest, uint32_t num_bytes){
	int i;
	char* source = (char*) vsource;
	char* dest = (char*) vdest;
	for(i = 0; i < num_bytes; i++){
		*(dest+i) = *(source+i);
	}
}