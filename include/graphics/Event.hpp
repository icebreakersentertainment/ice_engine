#ifndef EVENT_H_
#define EVENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/Keyboard.hpp"

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
	glm::detail::uint32 type;
};

struct KeyboardEvent
{
	glm::detail::uint32 type;
	glm::detail::uint8 state;
	glm::detail::uint8 repeat;
	glm::detail::uint8 padding2;
	glm::detail::uint8 padding3;
	KeySym keySym;
};

struct MouseMotionEvent
{
	glm::detail::uint32 type;
	glm::detail::uint32 state;
	glm::detail::uint8 padding1;
	glm::detail::uint8 padding2;
	glm::detail::uint8 padding3;
	glm::detail::int32 x;
	glm::detail::int32 y;
	glm::detail::int32 xrel;
	glm::detail::int32 yrel;
};

struct MouseButtonEvent
{
	glm::detail::uint32 type;
	glm::detail::uint8 button;
	glm::detail::uint8 state;
	glm::detail::uint8 padding2;
	glm::detail::uint8 padding3;
	glm::detail::int32 x;
	glm::detail::int32 y;
};

struct MouseWheelEvent
{
	glm::detail::uint32 type;
	glm::detail::int32 x;
	glm::detail::int32 y;
};

union Event
{
	glm::detail::uint32 type;
	GenericEvent generic;
	KeyboardEvent key;
	MouseMotionEvent motion;
	MouseButtonEvent button;
	MouseWheelEvent wheel;
};

}

#endif /* EVENT_H_ */
