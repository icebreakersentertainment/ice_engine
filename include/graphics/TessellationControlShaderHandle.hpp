#ifndef TESSELLATION_CONTROL_SHADER_HANDLE_H_
#define TESSELLATION_CONTROL_SHADER_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TessellationControlShaderHandle : public handles::Handle<TessellationControlShaderHandle>
{
public:
	using handles::Handle<TessellationControlShaderHandle>::Handle;
};

}

}

#endif /* TESSELLATION_CONTROL_SHADER_HANDLE_H_ */
