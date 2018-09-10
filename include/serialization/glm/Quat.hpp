#ifndef GLMQUATSERIALIZE_H_
#define GLMQUATSERIALIZE_H_

#include <boost/serialization/serialization.hpp>

#define GLM_FORCE_RADIANS
#include "glm/gtc/quaternion.hpp"

namespace boost
{
namespace serialization
{

template<class Archive> void serialize(Archive& ar, glm::quat& q, const unsigned int version)
{
	ar & q.x & q.y & q.z & q.w;
}

}
}

#endif /* GLMQUATSERIALIZE_H_ */
