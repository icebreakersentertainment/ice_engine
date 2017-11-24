#ifndef GHOSTOBJECTCOMPONENT_H_
#define GHOSTOBJECTCOMPONENT_H_

#include "physics/GhostObjectHandle.hpp"

namespace hercules
{
namespace entities
{

struct GhostObjectComponent
{
	GhostObjectComponent() = default;
	
	GhostObjectComponent(physics::GhostObjectHandle ghostObjectHandle) : ghostObjectHandle(ghostObjectHandle)
	{
	};
	
	physics::GhostObjectHandle ghostObjectHandle;
};

}
}

#endif /* GHOSTOBJECTCOMPONENT_H_ */
