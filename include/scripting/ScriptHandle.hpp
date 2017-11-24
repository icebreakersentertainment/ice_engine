#ifndef SCRIPT_HANDLE_H_
#define SCRIPT_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptHandle : public handles::Handle<ScriptHandle>
{
public:
	using handles::Handle<ScriptHandle>::Handle;
};

}
}

#endif /* SCRIPT_HANDLE_H_ */
