#include "../kernel/system.h"

void clock(){
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  bool is_am = true;

  putchar('\n');

  while(1){
    switch(hours){
      case 0:
        putchar('1');
        putchar('2');
        break;
      case 11:
        putchar('1');
        putchar('1');
        break;
      case 10:
        putchar('1');
        putchar('0');
        break;
      default:
        putchar('0');
        putchar('0' + hours);
        break;
    }

    putchar(':');

    putchar('0' + (minutes / 10));
    putchar('0' + (minutes % 10));

    putchar(':');

    putchar('0' + (seconds / 10));
    putchar('0' + (seconds % 10));

    if(is_am){
      putchar('a');
    } else{
      putchar('p');
    }
    putchar('m');

    seconds = (seconds + 1) % 60;

    if(seconds == 0){
      minutes = (minutes + 1) % 60;

      if(minutes == 0){
        hours = (hours + 1) % 12;

        if(hours == 0){
          is_am = !is_am;
        }
      }
    }

    sleep(1000);

    for(int i = 0; i < 10; i++){
      popchar();
    }
  }
}
