#ifndef MODEL_INSTANCE_HANDLE_H_
#define MODEL_INSTANCE_HANDLE_H_

#include "ResourceHandle.hpp"

namespace ice_engine
{
namespace graphics
{

class ModelInstanceHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ModelInstanceHandle INVALID;
};

}
}

#endif /* MODEL_INSTANCE_HANDLE_H_ */
