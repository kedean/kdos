#include "../io/screen_io.h"
#include "../kernel/types.h"
#include "../drivers/screen.h"

#include <stdarg.h>

void screen_init(){
	
}

void itoa(int value, char* buffer, int base){
	static const int BASE_DIGIT = 48;
	static const int HEX_ALPHA_BASE = 97 - 10;

	char reversed_digits[256];
	int digit_pointer = 0;

	if(value == 0){
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	else{
		int i = 0;
		switch(base){
			case 2:
				buffer[0] = '0';
				buffer[1] = 'b';
				i = 2;
				break;
			case 16:
				buffer[0] = '0';
				buffer[1] = 'x';
				i = 2;
				break;
			case 10:
				if(value < 0){ //deal with negative values
					buffer[0] = '-';
					i = 1;
					value = 0 - value;
				}
				break;
			default:
				return;
		}

		//do the computation and place values in reverse into a temporary buffer
		while(value > 0){
			uint8_t digit = (value % base);
			if(base == 16){
				reversed_digits[digit_pointer] = digit + (digit < 10 ? BASE_DIGIT : HEX_ALPHA_BASE);
			} else{
				reversed_digits[digit_pointer] = digit + BASE_DIGIT;
			}
			digit_pointer++;
			value /= base;
		}

		for(digit_pointer -= 1; digit_pointer >= 0; i++, digit_pointer--){
			buffer[i] = reversed_digits[digit_pointer];
		}
		buffer[i] = '\0';
	}
}

void uitoa(uint_t value, char* buffer){
	static const int BASE_DIGIT = 48;

	char reversed_digits[256];
	int digit_pointer = 0;

	if(value == 0){
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	else{
		int i = 0;

		//do the computation and place values in reverse into a temporary buffer
		while(value > 0){
			uint8_t digit = (value % 10);
			reversed_digits[digit_pointer] = digit + BASE_DIGIT;
			digit_pointer++;
			value /= 10;
		}

		for(digit_pointer -= 1; digit_pointer >= 0; i++, digit_pointer--){
			buffer[i] = reversed_digits[digit_pointer];
		}
		buffer[i] = '\0';
	}
}

void printf(const char* format, ...){
	va_list argptr;
	va_start(argptr, format);
	int int_value;
	char* string_value;
	char storage[256];

	for(const char* p = format; *p != '\0'; p++){
		if(*p != '%'){
			print_char(*p, -1, -1, WHITE_ON_BLACK);
		}
		else{
			switch(*++p){
				case 'u': //unsigned int handler
					int_value = va_arg(argptr, int);
					uitoa(int_value, storage);
					printf(storage);
					break;
				case 'd': //integer handler
					int_value = va_arg(argptr, int);
					itoa(int_value, storage, 10);
					printf(storage);
					break;
				case 'c': //char handler
					int_value = va_arg(argptr, int);
					print_char(int_value, -1, -1, WHITE_ON_BLACK);
					break;
				case 'x': //hexadecimal handler
					int_value = va_arg(argptr, int);
					itoa(int_value, storage, 16);
					printf(storage);
					break;
				case 's': //string handler
					string_value = va_arg(argptr, char*);
					printf(string_value);
					break;
				default: //if we dont' understand the format specifier, treat it as regular chars
					print_char('%', -1, -1, WHITE_ON_BLACK);
					print_char(*p, -1, -1, WHITE_ON_BLACK);
					break;
			}
		}
	}
}
