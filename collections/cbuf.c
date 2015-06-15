#include "../collections/cbuf.h"
#include "../memory/memory.h"
#include "../kernel/types.h"

static inline void increment_head(cbuf_s* buffer){
	(buffer->head)++;

	if(buffer->head > buffer->last){ //if the head overflows the buffer, wrap it around to the start
		buffer->head = buffer->first;
	}
}

static inline void decrement_head(cbuf_s* buffer){
	(buffer->head)--;

	if(buffer->head < buffer->first){ //if the head underflows the buffer, wrap it around to the end
		buffer->head = buffer->last;
	}
}

cbuf_s* cbuf_create(uint_t max_elements){
	size_t buffer_size = (sizeof(char) * max_elements);
	cbuf_s* buffer = (cbuf_s*) malloc(sizeof(cbuf_s) + buffer_size);
	buffer->first = (char*) ((size_t) buffer + sizeof(cbuf_s));
	buffer->head = buffer->first; //initially the head is at the start of the buffer
	buffer->last = (char*) ((size_t) buffer->first + buffer_size - 1);

	return buffer;
}

void cbuf_append(cbuf_s* buffer, char value){
	*(buffer->head) = value; //place the value in the head of the list
	
	increment_head(buffer);
}

void cbuf_pop(cbuf_s* buffer, char* destination, uint_t value_count){
	int d_index = 0; //position in the destination array
	decrement_head(buffer); //since the head marks the next element to be used, we need to rewind by one first

	for(; value_count > 0; value_count--){
		destination[d_index++] = *(buffer->head);
		decrement_head(buffer);
	}

	increment_head(buffer); //to compensate for the initial decrement
}

void cbuf_peek(cbuf_s* buffer, char* destination, uint_t value_count){
	char* orig_head = buffer->head; //store the head

	cbuf_pop(buffer, destination, value_count);

	buffer->head = orig_head; //restore the head
}

void cbuf_delete(cbuf_s* buffer){
	free(buffer);
}