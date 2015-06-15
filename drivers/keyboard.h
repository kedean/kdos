#pragma once
#include "../kernel/system.h"

typedef enum key_event_type key_event_type_e;
typedef struct key_event key_event_s;
typedef enum special_keys special_keys_e;

/**
 * Initializes the keyboard driver
 */
void keyboard_init();

/**
 * Fetch a single key from the keyboard buffer. Blocks until a value is returned.
 */
char get_key();

/**
 * Determines if the given value is a 'special' key. True for any non-printable value.
 * @param code the keycode to check
 */
bool is_special_key(uint8_t code);

/**
 * Maps a key code between its keyboard value and its ascii value
 * @param code the keycode to translate
 */
uint8_t map_key_code(uint8_t code);

enum special_keys{
	K_Escape=128,
	K_Backspace=129,
	K_Tab=130,
	K_Return=131,
	K_LeftCtrl=132,
	K_LeftShift=133,
	K_RightShift=134,
	K_LeftAlt=135,
	K_CapsLock=136,
	K_PadStar=137,
	K_F1=138,
	K_F2=139,
	K_F3=140,
	K_F4=141,
	K_F5=142,
	K_F6=143,
	K_F7=144,
	K_F8=145,
	K_F9=146,
	K_F10=147,
	K_F11=148,
	K_F12=149,
	K_NumLock=150,
	K_ScrollLock=151,
	K_Pad0=152,
	K_Pad1=153,
	K_Pad2=154,
	K_Pad3=155,
	K_Pad4=156,
	K_Pad5=157,
	K_Pad6=158,
	K_Pad7=159,
	K_Pad8=160,
	K_Pad9=161,
	K_PadMinus=162,
	K_PadPlus=163,
	K_PadDot=164
};

enum key_event_type {
	KeyDown = 0,
	KeyUp = 1
};

struct key_event{
	key_event_type_e type;
	uint8_t value;
	uint8_t code;
};