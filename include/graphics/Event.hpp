#ifndef EVENT_H_
#define EVENT_H_

#include "Types.hpp"

#include "graphics/Keyboard.hpp"

namespace hercules
{
namespace graphics
{

enum EventType
{
	UNKNOWN = 0,
	
	QUIT,
	
	KEYDOWN,
	KEYUP,
	
	MOUSEMOTION,
	MOUSEBUTTONDOWN,
	MOUSEBUTTONUP,
	MOUSEWHEEL
};

struct GenericEvent
{
	uint32 type;
};

struct KeyboardEvent
{
	uint32 type;
	uint8 state;
	uint8 repeat;
	uint8 padding2;
	uint8 padding3;
	KeySym keySym;
};

struct MouseMotionEvent
{
	uint32 type;
	uint32 state;
	uint8 padding1;
	uint8 padding2;
	uint8 padding3;
	int32 x;
	int32 y;
	int32 xrel;
	int32 yrel;
};

struct MouseButtonEvent
{
	uint32 type;
	uint8 button;
	uint8 state;
	uint8 padding2;
	uint8 padding3;
	int32 x;
	int32 y;
};

struct MouseWheelEvent
{
	uint32 type;
	int32 x;
	int32 y;
};

union Event
{
	uint32 type;
	GenericEvent generic;
	KeyboardEvent key;
	MouseMotionEvent motion;
	MouseButtonEvent button;
	MouseWheelEvent wheel;
};

}
}

#endif /* EVENT_H_ */
