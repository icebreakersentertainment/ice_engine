#ifndef EVENT_H_
#define EVENT_H_

#include "Types.hpp"

#include "graphics/Video.hpp"
#include "graphics/Keyboard.hpp"
#include "graphics/Mouse.hpp"

namespace ice_engine
{
namespace graphics
{

// Adapted from SDL2 - include/SDL_events.h
// https://www.libsdl.org/
/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
constexpr uint8 TEXTINPUTEVENT_TEXT_SIZE = 32;

enum State
{
	RELEASED = 0,
	PRESSED
};

enum EventType
{
	UNKNOWN = 0,
	
	QUIT,
	
	WINDOWEVENT,

    TEXTINPUT,
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

struct WindowEvent
{
	uint32 type;
	uint32 timestamp;
	uint32 windowId;
	WindowEventType eventType;
	uint8 padding1;
	uint8 padding2;
	uint8 padding3;
	sint32 data1;
	sint32 data2;
};

struct TextInputEvent
{
	uint32 type;
    uint32 timestamp;
    char text[TEXTINPUTEVENT_TEXT_SIZE];
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
	uint8 clicks;
	uint8 padding3;
	int32 x;
	int32 y;
};

struct MouseWheelEvent
{
	uint32 type;
	int32 x;
	int32 y;
	uint32 direction;
};

union Event
{
	uint32 type;
	GenericEvent generic;
	WindowEvent window;
	KeyboardEvent key;
	TextInputEvent text;
	MouseMotionEvent motion;
	MouseButtonEvent button;
	MouseWheelEvent wheel;
};

}
}

#endif /* EVENT_H_ */
