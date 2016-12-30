#ifndef ASSIMPUTILITIES_H_
#define ASSIMPUTILITIES_H_

#include <assimp/scene.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace utilities
{
	
void color4ToVec4(const aiColor4D* c, glm::vec4& v)
{
	v[0] = c->r;
	v[1] = c->g;
	v[2] = c->b;
	v[3] = c->a;
}

}

#endif /* ASSIMPUTILITIES_H_ */
