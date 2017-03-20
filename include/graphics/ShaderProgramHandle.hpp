#ifndef SHADER_PROGRAM_HANDLE_H_
#define SHADER_PROGRAM_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class ShaderProgramHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ShaderProgramHandle INVALID;
};

}

}

#endif /* SHADER_PROGRAM_HANDLE_H_ */
