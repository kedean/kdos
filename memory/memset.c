#include "../memory/memory.h"

void memset(void* vlocation, uint8_t value, uint32_t num_bytes){
	char* location = (char*) vlocation;
	for(int i = 0; i < num_bytes; i++){
		*(location+i) = value;
	}
}