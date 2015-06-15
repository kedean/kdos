#pragma once

#include "../kernel/types.h"

typedef struct PAGE_FRAME_DESCRIPTOR PAGE_FRAME_DESCRIPTOR;

struct PAGE_FRAME_DESCRIPTOR{
	bool is_in_use;
	void* frame;
};

/**
 * Creates and initalizes the heap of page frame metadata
 * @param init_heap the location to place the page frame heap
 * @param init_heap_size the size to allow the metadata to grow to, this determines how many pages may be allotted
 */
void heap_init_page_frame_heap(void* init_heap, uint_t init_heap_size);

/**
 * Allocates n page frames
 * @param frame_count number of page frames to allocate
 */
void* heap_alloc_page_frames(int frame_count);

/**
 * Free n page frames
 * @param start the address of the first page frame to free
 * @param frame_count the number of frames to free after the starting frame
 */
void heap_free_page_frames(void* start, int frame_count);

/**
 * Returns the location of the heap metadata, as set in heap_init_page_frame_heap
 */
void* heap_get_metadata_ptr();

/**
 * Returns the size of the heap metadata array, as set in heap_init_page_frame_heap
 */
uint32_t heap_get_metadata_size();

/**
 * The size of a page frame
 */
#ifndef PAGE_SIZE_BYTES
	#define PAGE_SIZE_BYTES 4096
#endif