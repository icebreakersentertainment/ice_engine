#ifndef SCRIPT_HANDLE_H_
#define SCRIPT_HANDLE_H_

#include "scripting/ResourceHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ScriptHandle INVALID;
};

}
}

#endif /* SCRIPT_HANDLE_H_ */
