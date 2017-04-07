#ifndef MODEL_HANDLE_H_
#define MODEL_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{

class ModelHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ModelHandle INVALID;
};

}

#endif /* MODEL_HANDLE_H_ */
