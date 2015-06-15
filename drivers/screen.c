#include "../kernel/ports.h"
#include "../memory/memory.h"
#include "../drivers/screen.h"
#include "../kernel/interrupts.h"
#include "../synch/synch.h"

/*
 * Computes the location in video memory of the given screen coordinates
 */
int get_screen_offset(int col, int row){
	return (2 * ((row * MAX_COLS) + col));
}

/*
 * Computes the location in video memory of the current cursor position
 */
int get_cursor(){
	//the control register is the devices index
	//register 14 is the high byte of the cursor offset
	//register 15 is the low byte of the cursor offset
	//select each register and read the byte into the desired portion of the offset
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	//offset given by VGA is number of characters, multiple by two because each char is two bytes
	return (offset * 2);
}

/*
 * Tells the video system where to place the cursor
 */
void set_cursor(int offset){
	offset /= 2; //offset was given in bytes, divide by two to get character count
	//same as get_cursor, except this time sending the parts of the offset into the control registers
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset));
}

/*
 * If the end of the text buffer overflows the screen, this will move everything up by one
 */
int handle_scrolling(int cursor_offset){
	// dont do anything if the cursor isnt off screen
	if(cursor_offset < MAX_ROWS * MAX_COLS * 2){
		return cursor_offset;
	}

	int i;
	// move everything up one
	for(i = 0; i < MAX_ROWS; i++){
		memcpy((void*)(get_screen_offset(0,i) + VIDEO_ADDRESS),
					(void*)(get_screen_offset(0,i-1) + VIDEO_ADDRESS),
					MAX_COLS*2);
	}

	// blank out the last line
	char* last_line = (char*) get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
	for(i = 0; i < MAX_COLS*2; i++){
		last_line[i] = 0;
	}

	cursor_offset -= 2*MAX_COLS; //allow printing from the start of the last row

	return cursor_offset;
}

/*
 * Public functions
 */

basic_lock print_lock;

void print_char(char character, int col, int row, char attribute_byte){
	lock(&print_lock);

	unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
	int offset; //the location in video mem corresponding to the row/col position
	int rows;

	if(!attribute_byte){ //default it to our normal value
		attribute_byte = WHITE_ON_BLACK;
	}

	if(col >= 0 && row >= 0){
		offset = get_screen_offset(col, row);
	}
	else {
		offset = get_cursor();
	}

	//newlines correspond to advancing a full row
	if(character == '\n'){
		rows = offset / (2*MAX_COLS); //each col is two bytes
		offset = get_screen_offset(79, rows);
	}
	else{
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
	}

	//next character cell
	offset += 2;
	offset = handle_scrolling(offset); //if we need to scroll things, this will scroll for this offset
	set_cursor(offset);

	unlock(&print_lock);
}

void clear_screen(){
	int row, col;

	for(row = 0; row < MAX_ROWS; row++){
		for(col = 0; col < MAX_COLS; col++){
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0,0));
}

void remove_char(){
	int offset = get_cursor();
	offset -= 1;

	set_cursor(offset);
	putchar(' ');
	set_cursor(offset);
}

void putchar(char character){
	print_char(character, -1, -1, WHITE_ON_BLACK);
}
