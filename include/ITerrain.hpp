#ifndef ITERRAIN_H_
#define ITERRAIN_H_

#include <vector>

#include "pathfinding/CrowdHandle.hpp"

#include "Types.hpp"

namespace ice_engine
{

class ITerrain
{
public:
	virtual ~ITerrain()
	{
	}
	;
	
	virtual void tick(const float32 delta) = 0;
	virtual const std::vector<pathfinding::CrowdHandle>& crowds() const = 0;
};

}

#endif /* ITERRAIN_H_ */
