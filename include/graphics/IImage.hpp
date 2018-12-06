#ifndef IIMAGE_H_
#define IIMAGE_H_

#include <vector>

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
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

#endif /* IIMAGE_H_ */
