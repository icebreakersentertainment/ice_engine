#ifndef PHYSICS_IHEIGHTFIELD_H_
#define PHYSICS_IHEIGHTFIELD_H_

#include <vector>

#include "Types.hpp"

namespace ice_engine
{
namespace physics
{

class IHeightfield
{
public:
	virtual ~IHeightfield()
	{
	}
	;
	
	virtual const std::vector<char>& data() const = 0;
	virtual uint32 width() const = 0;
	virtual uint32 length() const = 0;
	virtual uint32 height() const = 0;
};

}
}

#endif /* PHYSICS_IHEIGHTFIELD_H_ */
