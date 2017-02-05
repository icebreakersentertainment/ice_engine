#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

typedef int32 KeyCode;

enum
{
	KEY_UNKNOWN		= 0,
	
	KEY_ESCAPE		= '\033'
};

enum ScanCode
{
	SCANCODE_UNKNOWN		= 0,
	
	SCANCODE_ESCAPE			= 41
};

enum KeyState
{
	KEY_PRESSED		= 1,
	KEY_RELEASED	= 0
};

struct KeySym
{
	ScanCode scancode;
	KeyCode sym;
	uint16 mod;
};

}

}

#endif /* KEYBOARD_H_ */
