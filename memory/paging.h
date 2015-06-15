#pragma once

/*
 * Initialize the virtual memory system and enable it
 */
void paging_init();

/*
 * Maps a memory address such that the virtual and physical addresses are identical
 */
void paging_map_identity_page(void* addr, int size);