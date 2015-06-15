#include "../io/screen_io.h"
#include "../drivers/keyboard.h"
#include "../collections/cbuf.h"

void echo(){
	cbuf_s* command_buffer = cbuf_create(4); //only command right now is exit, which is 4 characters, plus a newline

	while(1){
		char newchar = get_key();

		if(is_special_key(newchar)){
			if(newchar == -125){ //TODO: make this correct, shouldn't need to comapre to negative value
				putchar('\n');

				//check if buffer contains command
				char last_command[4];
				cbuf_peek(command_buffer, last_command, 4);

				//TODO: need string comparison
				if(last_command[3] == 'e' && last_command[2] == 'x' && last_command[1] == 'i' && last_command[0] == 't'){
					printf("\n-- Ending 'echo' --\n");
					halt();
				}
			}
		}
		else{
			putchar(newchar);
			cbuf_append(command_buffer, newchar);
		}
	}

	cbuf_delete(command_buffer);

}
