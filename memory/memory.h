#pragma once

#include "../kernel/types.h"
#include "../memory/liballoc.h"

/**
 * Copy bytes between two memory locations
 */
void memcpy(void* source, void* dest, uint32_t num_bytes);

/**
 * Set the values from location to location + num_bytes to the given byte value
 */
void memset(void* location, uint8_t value, uint32_t num_bytes);