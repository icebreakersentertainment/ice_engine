#ifndef SCRIPT_OBJECT_HANDLE_H_
#define SCRIPT_OBJECT_HANDLE_H_

#include "scripting/ResourceHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptObjectHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ScriptObjectHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_HANDLE_H_ */
