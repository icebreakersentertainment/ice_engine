#ifndef PATHFINDING_ITERRAIN_H_
#define PATHFINDING_ITERRAIN_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace pathfinding
{

class ITerrain
{
public:
	virtual ~ITerrain()
	{
	}
	;
	
	virtual const std::vector<glm::vec3>& vertices() const = 0;
	virtual const std::vector<uint32>& indices() const = 0;
};

}
}

#endif /* PATHFINDING_ITERRAIN_H_ */
