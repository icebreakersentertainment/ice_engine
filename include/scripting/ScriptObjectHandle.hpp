#ifndef SCRIPT_OBJECT_HANDLE_H_
#define SCRIPT_OBJECT_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptObjectHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ScriptObjectHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_HANDLE_H_ */
