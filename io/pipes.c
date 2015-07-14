#include "../kernel/system.h"
#include "../io/pipes.h"

pipe_entry_s* pipes;
uint_t num_pipes;

void pipe_init(uint_t max_pipes){
  uint32_t bytes = sizeof(pipe_entry_s) * max_pipes;
  pipes = malloc(bytes);
  memset(pipes, NULL, bytes); //nullify every pipe entry so they are all free, a null entry is an available pipe space
  num_pipes = max_pipes;
}

pipe_handle_t find_open_entry(){
  for(int i = 0; i < num_pipes; i++){
    if(pipes[i].buffer == NULL){ //free pipe space has been found
      return i;
    }
  }

  return -1;
}

pipe_handle_t pipe_create(uint_t buffer_size){
  pipe_handle_t index = find_open_entry();

  if(index == -1){
    return -1;
  } else{
    pipes[index].buffer = cbuf_create(buffer_size);
    pipes[index].type = BIDIRECTIONAL;
    pipes[index].parent_handle = -1;
    return index;
  }
}

void pipe_close(pipe_handle_t handle){
  if(pipes[handle].buffer != NULL){
    cbuf_delete(pipes[handle].buffer);
    pipes[handle].buffer = NULL;
    if(pipes[handle].parent_handle != -1){ //the parent relationship can only be one deep, as it is a wrapping relationship
      pipes[pipes[handle].parent_handle].buffer = NULL;
    }
  }
}

pipe_handle_t pipe_wrap_readonly(pipe_handle_t original){
  pipe_handle_t index = find_open_entry();

  if(index == -1){
    return -1;
  } else{
    pipes[index].buffer = pipes[original].buffer;
    pipes[index].type = READONLY;
    pipes[index].parent_handle = original;
    return index;
  }
}

pipe_handle_t pipe_wrap_writeonly(pipe_handle_t original){
  pipe_handle_t index = find_open_entry();

  if(index == -1){
    return -1;
  } else{
    pipes[index].buffer = pipes[original].buffer;
    pipes[index].type = WRITEONLY;
    pipes[index].parent_handle = original;
    return index;
  }
}

void pipe_push(pipe_handle_t handle, char c){
  cbuf_append(pipes[handle].buffer, c);
}

void pipe_write(pipe_handle_t handle, char* c, uint_t length){
  //TODO: reimplement this with a better cbuf
  for(int i = 0; i < length; i++){
    cbuf_append(pipes[handle].buffer, c[i]);
  }
}

char pipe_pop(pipe_handle_t handle){
  char out;
  cbuf_peek(pipes[handle].buffer, &out, 1);
  return out;
}

void pipe_read(pipe_handle_t handle, char* destination, uint_t length){
  cbuf_peek(pipes[handle].buffer, destination, length);
}

void pipe_flush(pipe_handle_t handle, uint_t length){
  char bucket[length];
  cbuf_pop(pipes[handle].buffer, bucket, length);
}
