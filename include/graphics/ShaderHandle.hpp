#ifndef SHADER_HANDLE_H_
#define SHADER_HANDLE_H_

#include <iostream>

#include "GraphicsResourceHandle.hpp"
#include "Types.hpp"

namespace hercules
{
namespace graphics
{

enum struct ShaderType
{
	UNKNOWN = 0,
	
	VERTEX,
	FRAGMENT,
	COMPUTE
};

class ShaderHandle : public GraphicsResourceHandle
{
public:
	static const ShaderHandle INVALID;
};

//const ShaderHandle ShaderHandle::INVALID = ShaderHandle();

}

}

#endif /* SHADER_HANDLE_H_ */
