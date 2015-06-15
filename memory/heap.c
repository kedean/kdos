#include "../kernel/system.h"

PAGE_FRAME_DESCRIPTOR* _base_heap_ptr;
uint_t _page_frame_count;

void* heap_get_metadata_ptr(){
	return _base_heap_ptr;
}

uint32_t heap_get_metadata_size(){
	return sizeof(PAGE_FRAME_DESCRIPTOR) * _page_frame_count;
}

void heap_init_page_frame_heap(void* init_heap, uint_t init_heap_size){
	_base_heap_ptr = (PAGE_FRAME_DESCRIPTOR*) init_heap;
	_page_frame_count = init_heap_size / sizeof(PAGE_FRAME_DESCRIPTOR); //force the number of frames to be the largest amount that fits in the given size
	memloc_t heap_start = (memloc_t) _base_heap_ptr + _page_frame_count * sizeof(PAGE_FRAME_DESCRIPTOR); //the point where we can potentially begin allocating page frames
	memloc_t first_page_frame = (((int)heap_start) % PAGE_SIZE_BYTES == 0) ? heap_start : (memloc_t) (((((int)heap_start) / PAGE_SIZE_BYTES) + 1) * PAGE_SIZE_BYTES); //if heap_start is already a multiple of the frame size, start there, otherwise go with the next address that is a multiple of the frame size

	for(uint_t i = 0; i < _page_frame_count; i++){
		_base_heap_ptr[i].is_in_use = false;
		_base_heap_ptr[i].frame = first_page_frame + (i * PAGE_SIZE_BYTES);
	}
}

void* heap_alloc_page_frames(int frame_count){
	uint_t first_frame_in_sequence_index = -1;
	uint_t frames_found = 0;

	for(uint_t i = 0; i < _page_frame_count; i++){

		if(_base_heap_ptr[i].is_in_use == true){
			first_frame_in_sequence_index = -1;
			frames_found = 0;
		} else if(first_frame_in_sequence_index != -1){
			frames_found++;
		} else{
			first_frame_in_sequence_index = i;
		}

		if(frames_found == frame_count){
			break;
		}
	}

	if(first_frame_in_sequence_index == -1 || frames_found != frame_count){
		return 0;
	} else{
		for(int i = first_frame_in_sequence_index; i < first_frame_in_sequence_index + frames_found; i++){
			_base_heap_ptr[i].is_in_use = true;
			paging_map_identity_page(_base_heap_ptr[i].frame, PAGE_SIZE_BYTES); //id map everything! we don't have swapping yet
		}

		void* page = _base_heap_ptr[first_frame_in_sequence_index].frame;		
		return page;
	}
}

void heap_free_page_frames(void* start, int frame_count){
	for(uint_t i = 0; i < _page_frame_count; i++){
		if(frame_count == 0){
			break;
		} else if(_base_heap_ptr[i].frame >= start){
			_base_heap_ptr[i].is_in_use = false;
			frame_count--;
		}
	}
}
