#pragma once

#include "../io/pipes.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

//default color scheme
#define WHITE_ON_BLACK 0x0f

//screen io ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

/**
 * Print a character to a position on the screen.
 * @param character the ascii character to print
 * @param col the column to assign to
 * @param row the row to assign to
 * @param attribute_byte the attributes to give the character
 */
void print_char(char character, int col, int row, char attribute_byte);

/*
 * Removes all characters from the screen and resets the pointer
 */
void clear_screen();

/*
 * Removes the character just below the cursor, and retreats the cursor by 1
 */
void remove_char();

/**
 * Clears the screen and replaces it with the tail contents of the given pipe.
 * @param handle the pipe to read from
 */
void sync_screen_to_pipe(pipe_handle_t handle);
