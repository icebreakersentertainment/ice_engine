#ifndef GLMVEC3SERIALIZE_H_
#define GLMVEC3SERIALIZE_H_

#include <boost/serialization/serialization.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace boost
{
namespace serialization
{

template<class Archive> void serialize(Archive& ar, glm::vec3& v, const unsigned int version)
{
	ar & v.x & v.y & v.z;
}

}
}

#endif /* GLMVEC3SERIALIZE_H_ */
