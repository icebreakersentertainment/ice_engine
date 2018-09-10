#ifndef SCRIPTOBJECTCOMPONENT_H_
#define SCRIPTOBJECTCOMPONENT_H_

#include "scripting/ScriptObjectHandle.hpp"

namespace ice_engine
{
namespace ecs
{

struct ScriptObjectComponent
{
	ScriptObjectComponent() = default;
	
	ScriptObjectComponent(void* object) : scriptObjectHandle(scripting::ScriptObjectHandle(object))
	{
	};

	ScriptObjectComponent(scripting::ScriptObjectHandle scriptObjectHandle1) : scriptObjectHandle(scriptObjectHandle1)
	{
	};
	
	scripting::ScriptObjectHandle scriptObjectHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::ScriptObjectComponent& c, const unsigned int version)
{
	ar & c.scriptObjectHandle;
}

}
}

#endif /* SCRIPTOBJECTCOMPONENT_H_ */
