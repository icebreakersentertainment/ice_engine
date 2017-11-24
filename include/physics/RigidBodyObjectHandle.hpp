#ifndef RIGID_BODY_OBJECT_HANDLE_H_
#define RIGID_BODY_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace hercules
{
namespace physics
{

class RigidBodyObjectHandle : public handles::PointerHandle
{
public:
	using handles::PointerHandle::PointerHandle;
	
	static const RigidBodyObjectHandle INVALID;
};

}
}

#endif /* RIGID_BODY_OBJECT_HANDLE_H_ */
