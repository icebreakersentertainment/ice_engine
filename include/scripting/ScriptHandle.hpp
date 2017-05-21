#ifndef SCRIPT_HANDLE_H_
#define SCRIPT_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ScriptHandle INVALID;
};

}
}

#endif /* SCRIPT_HANDLE_H_ */
