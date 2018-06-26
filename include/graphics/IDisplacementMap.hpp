#ifndef IDISPLACEMENT_H_
#define IDISPLACEMENT_H_

#include "graphics/IImage.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class IDisplacementMap
{
public:
	virtual ~IDisplacementMap()
	{
	}
	;
	
	//virtual const IImage* image() const = 0;
};

}
}

#endif /* IDISPLACEMENT_H_ */
