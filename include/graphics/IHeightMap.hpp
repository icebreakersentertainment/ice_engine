#ifndef IHEIGHTMAP_H_
#define IHEIGHTMAP_H_

#include "graphics/IImage.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class IHeightMap
{
public:
	virtual ~IHeightMap()
	{
	}
	;
	
	virtual const IImage* image() const = 0;
};

}
}

#endif /* IHEIGHTMAP_H_ */
