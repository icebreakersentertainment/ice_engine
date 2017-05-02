#ifndef SHADER_PROGRAM_HANDLE_H_
#define SHADER_PROGRAM_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class ShaderProgramHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ShaderProgramHandle INVALID;
};

}

}

#endif /* SHADER_PROGRAM_HANDLE_H_ */
