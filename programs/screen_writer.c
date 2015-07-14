/**
 * This program synchronizes the screen buffer in io/screen_io.c with the actual VGA driver. It should be run in its own thread.
 */

#include "../drivers/screen.h"
#include "../kernel/system.h"

void screen_writer(){
  const uint_t buffer_size = 2000;
  char buffer[buffer_size]; //up to 2k characters can be on screen at a given moment

  for(;;){
    /*
     * TODO: This can be greatly improved by somehow accessing the video memory directly. Find a way to do this gracefully, without just getting the pointer manually.
     */

    screen_get_stdout_content(buffer, buffer_size);

    clear_screen();

    for(int i = 0; i < buffer_size; i++){
      print_char(buffer[i], -1, -1, WHITE_ON_BLACK);
    }
    sleep(10);
  }
}
