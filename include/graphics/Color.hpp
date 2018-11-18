#ifndef COLOR_H_
#define COLOR_H_

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

namespace
{
inline uint32 rgbaToUInt32(const uint8 r, const uint8 g, const uint8 b, const uint8 a)
{
	return (a << 24) + (b << 16) + (g << 8) + r;
}
}

class Color
{
public:
	Color() = default;
	Color(const uint32 color) : color_(color)
	{
	}
	Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a)
	:
		color_(rgbaToUInt32(r, g, b, a))
	{
	}

	void setColor(const uint32 color)
	{
		color_ = color;
	}

	void setColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a)
	{
		color_ = rgbaToUInt32(r, g, b, a);
	}

	uint32 color() const
	{
		return color_;
	}

private:
	uint32 color_ = 0;
};

}
}

#endif /* COLOR_H_ */
