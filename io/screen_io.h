#pragma once

#include "../kernel/types.h"

/**
 * Initialize the base screen pipe
 */
void screen_init();

/**
 * Fetches the current contents of stdout into a sized buffer.
 * @param destination the buffer to inject the data into
 * @param length the number of bytes to fetch
 */
void screen_get_stdout_content(char* destination, uint_t length);

/**
 * Computes the string representation of the given value and places it into a buffer
 * @param value the value to stringify
 * @param buffer the character buffer to place the result in, must be large enough to accomodate
 * @param base the radix to convert with, unknown radixes are undefined behavior. Supported: 2, 10, 16
 */
void itoa(int value, char* buffer, int base);

/**
 * Computes the string representation of the given decimal values and places it into a buffer.
 * Treats all input as unsigned.
 * @param value the value to stringify
 * @param buffer the character buffer to place the result in, must be large enough to accomodate
 */
void uitoa(uint_t value, char* buffer);

/**
 * Prints a string to the screen using standard formatting
 * @param format the format string
 */
void printf(const char* format, ...);

/*
 * Prints the character to the screen at the current cursor location, and advances the cursor
 */
void putchar(char character);

/**
 * Removes one character from the screen and returns it
 * @return the last character on screen
 */
char popchar();
