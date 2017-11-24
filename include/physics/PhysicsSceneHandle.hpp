#ifndef PHYSICS_SCENE_HANDLE_H_
#define PHYSICS_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace physics
{

class PhysicsSceneHandle : public handles::Handle<PhysicsSceneHandle>
{
public:
	using handles::Handle<PhysicsSceneHandle>::Handle;
};

}
}

#endif /* PHYSICS_SCENE_HANDLE_H_ */
