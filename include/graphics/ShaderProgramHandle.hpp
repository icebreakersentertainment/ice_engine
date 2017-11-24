#ifndef SHADER_PROGRAM_HANDLE_H_
#define SHADER_PROGRAM_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class ShaderProgramHandle : public handles::Handle<ShaderProgramHandle>
{
public:
	using handles::Handle<ShaderProgramHandle>::Handle;
};

}

}

#endif /* SHADER_PROGRAM_HANDLE_H_ */
