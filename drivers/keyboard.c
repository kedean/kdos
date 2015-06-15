#include "../drivers/keyboard.h"
#include "../kernel/system.h"
#include "../kernel/ports.h"

char keyboard_buffer[] = "foobar";
int keyboard_buffer_end = -1;
basic_lock keyboard_lock; //so that the handler can push keys to the buffer without get_key immediately consuming it

void keyboard_handle_keys(interrupt_data_s* r){
	lock(&keyboard_lock);

	uint8_t c = port_byte_in(0x60);

	key_event_s keypress;

	if(c >= 128){
		keypress.type = KeyUp;
		c -= 128;
	}
	else{
		keypress.type = KeyDown;
	}

	keypress.value = map_key_code(c);

	if(keypress.type == KeyDown){
		keyboard_buffer_end += 1;
		keyboard_buffer[keyboard_buffer_end] = keypress.value;
	}

	unlock(&keyboard_lock);
}

void keyboard_init(){
	interrupts_register_callback(INT_KEYBOARD, &keyboard_handle_keys);
}

char get_key(){
	while(keyboard_buffer_end == -1);

	lock(&keyboard_lock);
	char output = keyboard_buffer[keyboard_buffer_end--];
	unlock(&keyboard_lock);
	return output;
}

bool is_special_key(uint8_t key){
	if (key > 127 || key == 0){
		return true;
	}
	else{
		return false;
	}
}

uint8_t map_key_code(uint8_t code){
	static const uint8_t code_mapping[] = {
		0, K_Escape, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'-', '=', K_Backspace, K_Tab,
		'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
		K_Return, K_LeftCtrl,
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
		'\'', '`', K_LeftShift, '\\',
		'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
		K_RightShift, K_PadStar, K_LeftAlt, ' ', K_CapsLock,
		K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10,
		K_NumLock, K_ScrollLock, K_Pad7, K_Pad8, K_Pad9, K_PadMinus, K_Pad4,
		K_Pad5, K_Pad6, K_PadPlus, K_Pad1, K_Pad2, K_Pad3, K_Pad0, K_PadDot,
		0, 0, 0, K_F11, K_F12
		};
	return code_mapping[code];
}
