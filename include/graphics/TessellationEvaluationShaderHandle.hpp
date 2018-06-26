#ifndef TESSELLATION_EVALUATION_SHADER_HANDLE_H_
#define TESSELLATION_EVALUATION_SHADER_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TessellationEvaluationShaderHandle : public handles::Handle<TessellationEvaluationShaderHandle>
{
public:
	using handles::Handle<TessellationEvaluationShaderHandle>::Handle;
};

}

}

#endif /* TESSELLATION_EVALUATION_SHADER_HANDLE_H_ */
