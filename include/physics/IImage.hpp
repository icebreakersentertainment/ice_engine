#ifndef PHYSICS_IIMAGE_H_
#define PHYSICS_IIMAGE_H_

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

	virtual const std::vector<byte>& data() const = 0;
	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual int32 format() const = 0;
};

}
}

#endif /* PHYSICS_IIMAGE_H_ */
