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
	
	ScriptObjectComponent(scripting::ScriptObjectHandle scriptObjectHandle) : scriptObjectHandle(scriptObjectHandle)
	{
	};
	
	scripting::ScriptObjectHandle scriptObjectHandle;
};

}
}

#endif /* SCRIPTOBJECTCOMPONENT_H_ */
