#ifndef IPHYSICSENGINE_H_
#define IPHYSICSENGINE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "ICollisionShape.hpp"

namespace hercules
{
namespace physics
{

class IPhysicsEngine
{
public:
	virtual ~IPhysicsEngine()
	{
	}
	;
	
	virtual void tick(float32 delta) = 0;
	
	virtual ICollisionShape* createStaticPlane(const glm::vec3& planeNormal, float32 planeConstant) = 0;
};

}
}

#endif /* IPHYSICSENGINE_H_ */
