#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace graphics
{

typedef glm::detail::int32 KeyCode;

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
	glm::detail::uint16 mod;
};

}

#endif /* KEYBOARD_H_ */
