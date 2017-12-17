#ifndef GHOST_OBJECT_HANDLE_H_
#define GHOST_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace physics
{

class GhostObjectHandle : public handles::PointerHandle
{
public:
	using handles::PointerHandle::PointerHandle;
	
	static const GhostObjectHandle INVALID;
};

}
}

#endif /* GHOST_OBJECT_HANDLE_H_ */
