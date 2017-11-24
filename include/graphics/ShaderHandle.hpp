#ifndef SHADER_HANDLE_H_
#define SHADER_HANDLE_H_

#include "handles/Handle.hpp"

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

class ShaderHandle : public handles::Handle<ShaderHandle>
{
public:
	using handles::Handle<ShaderHandle>::Handle;
};

}

}

#endif /* SHADER_HANDLE_H_ */
