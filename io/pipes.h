#pragma once

#include "../collections/cbuf.h"
#include "../kernel/types.h"

typedef uint_t pipe_handle_t;
typedef struct pipe_entry pipe_entry_s;

struct pipe_entry {
  cbuf_s* buffer;
  pipe_handle_t parent_handle;
  enum {
    BIDIRECTIONAL=0,
    READONLY=1,
    WRITEONLY=2
  } type;
};

/**
 * Initializes the pipe tracking system.
 * Prerequisites: dynamic memory allocation (malloc)
 * @param max_pipes the maximum number of pipes that may exist at one time. bi-directional, read-only, and write-only versions of a pipe count as individuals.
 */
void pipe_init(uint_t max_pipes);

/**
 * Creates a new bi-directional pipe with buffer_size entries. Pipes are implemented as a circular buffer, and will overwrite their ends.
 * @param buffer_size the maximum amount of data stored before overwriting occurs
 * @return a handle to a pipe, -1 if it cannot be created
 */
pipe_handle_t pipe_create(uint_t buffer_size);

/**
 * Close an existing pipe and free its entry in the pipe table.
 * @param handle the pipe to close
 */
void pipe_close(pipe_handle_t handle);

/**
 * Creates a handle to a read-only version of a pipe. If pipe_write is called with this handle, it will fail.
 * @param original a handle to an existing bi-directional pipe
 * @return a handle to a pipe, -1 if the wrapper cannot be created
 */
pipe_handle_t pipe_wrap_readonly(pipe_handle_t original);

/**
 * Creates a handle to a write-only version of a pipe. If pipe_pop or pipe_read is called with this handle, it will fail.
 * @param original a handle to an existing bi-directional pipe
 * @return a handle to a pipe, -1 if the wrapper cannot be created
 */
pipe_handle_t pipe_wrap_writeonly(pipe_handle_t original);

/**
 * Write a single byte to the given pipe.
 * @param handle the pipe to write to
 * @param c the character to write
 */
void pipe_push(pipe_handle_t handle, char c);
/**
 * Write a stream of bytes to the given pipe.
 * @param handle the pipe to write to
 * @param c the character array to write
 * @param length the number of characters stored in c
 */
void pipe_write(pipe_handle_t handle, char* c, uint_t length);

/**
 * Removes and returns a single character from a pipe.
 * @param handle the pipe to read from
 * @return a single character from the pipe
 */
char pipe_pop(pipe_handle_t handle);

/**
 * Removes and returns the given number of characters from a pipe.
 * @param handle the pipe to read from
 * @param destination an existing character buffer to place the characters in
 * @param length the number of characters to read
 */
void pipe_read(pipe_handle_t handle, char* destination, uint_t length);

/**
 * Flushes n characters from the mouth of the pipe
 * @param handle the pipe to flush
 * @param length the number of characters to flush
 */
void pipe_flush(pipe_handle_t handle, uint_t length);
