/*
===============================================================================

	XOF
	===
	File	:	XOF_Keyboard_I.hpp
	Desc	:	Basic keyboard interface, implement using some API.

===============================================================================
*/
#ifndef XOF_KEYBOARD_I
#define XOF_KEYBOARD_I

#include "../Platform/XOF_Platform.hpp"

// Intended to be used for indexing into an OS/API specific keyCode array.
// Game/Application code should be able to use these enum values and be shielded from OS/API specifics.
enum XOF_KEYBOARD {
	// esc
	ESCAPE = 0,

	// function keys
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	// numerical keys
	NUMBER_0, NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4, NUMBER_5, NUMBER_6, NUMBER_7, NUMBER_8, NUMBER_9,

	// alphabetic keys
	A, B, C, D, E, F, G, H, I, J, K, L, M, 
	N, O, P, Q, R, S, T, U ,V, W, X, Y, Z,

	// numpad
	NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9, 
	NUMPAD_DECIMAL, NUMPAD_ENTER, NUMPAD_PLUS, NUMPAD_MINUS, NUMPAD_MULTIPLY, NUMPAD_DIVIDE, NUM_LOCK,

	// arrows
	ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT,

	// cluster 1
	PRINT_SCREEN, SCROLL_LOCK, PAUSE_BREAK,

	// cluster 2
	INSERT, HOME, DEL, END, PAGE_UP, PAGE_DOWN,

	// left & right variant keys
	LEFT_SHIFT, RIGHT_SHIFT, LEFT_CONTROL, RIGHT_CONTROL, LEFT_ALT, RIGHT_ALT, LEFT_WINDOWS, RIGHT_WINDOWS,

	// cluster 3
	TAB, CAPS_LOCK,
	BACK_SPACE, ENTER,
	SPACE_BAR, MENU,

	// cluster 4
	PLUS, MINUS,
	COMMA, PERIOD,

	// OEM keys - keyboard specific
	// ;:,    /?,    '@,    [{,    \|,    ]},    #~,   `�� (uk keyboard)
	OEM_1, OEM_2, OEM_3, OEM_4, OEM_5, OEM_6, OEM_7, OEM_8,

	KEY_COUNT,
};

class KeyboardI {
public:
	virtual			~KeyboardI() {}

	virtual void	Update() = 0;

	virtual bool	IsKeyUp( U32 key ) = 0;
	virtual bool	IsKeyDown( U32 key ) = 0;

private:

};

#endif // XOF_KEYBOARD_I