#ifndef GLMIVEC4SERIALIZE_H_
#define GLMIVEC4SERIALIZE_H_

#include <boost/serialization/serialization.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace boost
{
namespace serialization
{

template<class Archive> void serialize(Archive& ar, glm::ivec4& v, unsigned int version)
{
	ar & v.w & v.x & v.y & v.z;
}

}
}

#endif /* GLMIVEC4SERIALIZE_H_ */
