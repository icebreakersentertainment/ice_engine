#ifndef SHADER_HANDLE_H_
#define SHADER_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

enum struct ShaderType
{
	UNKNOWN = 0,
	
	VERTEX,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};

class ShaderHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
};

}

}

#endif /* SHADER_HANDLE_H_ */
