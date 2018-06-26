#ifndef GRAPHICS_MODEL_HANDLE_H_
#define GRAPHICS_MODEL_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class ModelHandle : public handles::Handle<ModelHandle>
{
public:
	using handles::Handle<ModelHandle>::Handle;
};

}
}

#endif /* GRAPHICS_MODEL_HANDLE_H_ */
