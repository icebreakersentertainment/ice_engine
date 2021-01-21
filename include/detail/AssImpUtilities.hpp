#ifndef ASSIMPUTILITIES_H_
#define ASSIMPUTILITIES_H_

#include <assimp/scene.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ice_engine
{
namespace detail
{

//inline void toGlm(const aiColor4D* c, glm::vec4& v)
//{
//	v[0] = c.r;
//	v[1] = c.g;
//	v[2] = c.b;
//	v[3] = c.a;
//}

inline glm::vec2 toGlm(const aiVector2D& v)
{
	return glm::vec2(v.x, v.y);
}

inline glm::vec3 toGlm(const aiVector3D& v)
{
	return glm::vec3(v.x, v.y, v.z);
}

inline glm::vec4 toGlm(const aiColor4D& c)
{
	return glm::vec4(c.r, c.g, c.b, c.a);
}

inline glm::mat4 toGlm(const aiMatrix4x4& m)
{
	return glm::mat4(
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4
	);
}

inline glm::quat toGlm(const aiQuaternion& q)
{
	return glm::quat(q.w, q.x, q.y, q.z);
}

}
}

#endif /* ASSIMPUTILITIES_H_ */
