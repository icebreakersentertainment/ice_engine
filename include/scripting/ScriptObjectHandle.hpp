#ifndef SCRIPT_OBJECT_HANDLE_H_
#define SCRIPT_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace scripting
{

class ScriptObjectHandle : public handles::PointerHandle
{
public:
	using handles::PointerHandle::PointerHandle;
	
	static const ScriptObjectHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_HANDLE_H_ */
