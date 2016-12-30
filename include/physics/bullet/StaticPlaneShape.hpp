#ifndef STATICPLANESHAPE_H_
#define STATICPLANESHAPE_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

#include "physics/ICollisionShape.hpp"

namespace physics
{
namespace bullet
{

class StaticPlaneShape : public ICollisionShape
{
public:
	StaticPlaneShape(const glm::vec3& planeNormal, glm::detail::float32 planeConstant);
	virtual ~StaticPlaneShape();

private:
	StaticPlaneShape(const StaticPlaneShape& other);
	
	std::unique_ptr<btCollisionShape> shape_;
};

}
}

#endif /* STATICPLANESHAPE_H_ */
