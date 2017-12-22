#ifndef VERTEX_SHADER_HANDLE_H_
#define VERTEX_SHADER_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class VertexShaderHandle : public handles::Handle<VertexShaderHandle>
{
public:
	using handles::Handle<VertexShaderHandle>::Handle;
};

}

}

#endif /* VERTEX_SHADER_HANDLE_H_ */
