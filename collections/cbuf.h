#pragma once

#include "../kernel/types.h"

/*
 * Defines functions for working with a circular buffer (cbuf) of single byte values.
 * If a circular buffer becomes full, it begins overwriting the oldest values.
 */

typedef struct cbuf cbuf_s;

struct cbuf{
	char* first;
	char* last;
	char* head;
};

/**
 * Allocates space for a new circular buffer that will overwrite old elements after max_elements appends.
 * The return value must be deleted with circular_buffer_delete!
 * @param max_elements The number of byte-size elements that may be appended before the buffer circles back
 */
cbuf_s* cbuf_create(uint_t max_elements);

/**
 * Appends a one-byte value to the buffer
 * @param buffer The buffer to be appended to
 * @param value The value to be appended
 */
void cbuf_append(cbuf_s* buffer, char value);

/**
 * Fetches the last n values to be pushed to the buffer and removes them from the buffer (by rewinding the head).
 * If value_count exceeds the number of elements in the buffer and the buffer is full, the result will include duplicate values.
 * If value_count exceeds the number of elements and the buffer is not full, the result will include garbage values.
 * @param buffer The buffer to pop from
 * @param destination The char array to put the values into, must be allocated already
 * @param value_count The number of values to pop
 */
void cbuf_pop(cbuf_s* buffer, char* destination, uint_t value_count);

/**
 * Fetches the last n values to be pushed to the buffer, the head remains unaffected. Subsequent calls will return identical results.
 * If value_count exceeds the number of elements in the buffer and the buffer is full, the result will include duplicate values.
 * If value_count exceeds the number of elements and the buffer is not full, the result will include garbage values.
 * @param buffer The buffer to peek from
 * @param destination The char array to put the values into, must be allocated already
 * @param value_count The number of values to peek
 */
void cbuf_peek(cbuf_s* buffer, char* destination, uint_t value_count);

/**
 * Frees the space allocated for a cbuf
 * @param buffer the buffer to delete
 */
void cbuf_delete(cbuf_s* buffer);