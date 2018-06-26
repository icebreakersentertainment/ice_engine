#ifndef PATHFINDING_IIMAGE_H_
#define PATHFINDING_IIMAGE_H_

#include <vector>

#include "Types.hpp"

namespace ice_engine
{
namespace physics
{

class IImage
{
public:
	enum Format
	{
		FORMAT_UNKNOWN = -1,
		FORMAT_RGB,
		FORMAT_RGBA
	};

	virtual ~IImage()
	{
	}
	;
	
	virtual const std::vector<char>& data() const = 0;
	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual Format format() const = 0;
};

}
}

#endif /* PATHFINDING_IIMAGE_H_ */
