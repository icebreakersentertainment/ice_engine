#ifndef FRAGMENT_SHADER_HANDLE_H_
#define FRAGMENT_SHADER_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class FragmentShaderHandle : public handles::Handle<FragmentShaderHandle>
{
public:
	using handles::Handle<FragmentShaderHandle>::Handle;
};

}

}

#endif /* FRAGMENT_SHADER_HANDLE_H_ */
