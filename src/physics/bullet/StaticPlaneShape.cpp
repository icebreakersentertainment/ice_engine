#include "physics/bullet/StaticPlaneShape.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

StaticPlaneShape::StaticPlaneShape(const glm::vec3& planeNormal, float32 planeConstant)
{
	 shape_ = std::unique_ptr<btCollisionShape>( new btStaticPlaneShape(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant) );
}


StaticPlaneShape::StaticPlaneShape(const StaticPlaneShape& other)
{
}

StaticPlaneShape::~StaticPlaneShape()
{
}

}
}
}
