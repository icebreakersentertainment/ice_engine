#ifndef ASSIMPUTILITIES_H_
#define ASSIMPUTILITIES_H_

#include <assimp/scene.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ice_engine
{
namespace detail
{

inline void color4ToVec4(const aiColor4D* c, glm::vec4& v)
{
	v[0] = c->r;
	v[1] = c->g;
	v[2] = c->b;
	v[3] = c->a;
}

inline glm::mat4 convertAssImpMatrix(const aiMatrix4x4* m)
{
	return glm::mat4(
		m->a1, m->b1, m->c1, m->d1,
		m->a2, m->b2, m->c2, m->d2,
		m->a3, m->b3, m->c3, m->d3,
		m->a4, m->b4, m->c4, m->d4
	);
}

}
}

#endif /* ASSIMPUTILITIES_H_ */
