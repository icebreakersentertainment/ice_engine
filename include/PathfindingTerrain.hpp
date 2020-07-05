#ifndef PATHFINDINGTERRAIN_H_
#define PATHFINDINGTERRAIN_H_

#include "pathfinding/ITerrain.hpp"

#include "HeightMap.hpp"

namespace ice_engine
{

class PathfindingTerrain : public pathfinding::ITerrain
{
public:

	PathfindingTerrain() = default;
	
	PathfindingTerrain(const HeightMap& heightMap)
	{
		generatePathfindingTerrain(heightMap);
	}
	
	virtual ~PathfindingTerrain() override = default;

	virtual const std::vector<glm::vec3>& vertices() const override
	{
		return vertices_;
	}

    virtual const std::vector<uint32>& indices() const override
    {
        return indices_;
    }

private:
	std::vector<glm::vec3> vertices_;
	std::vector<uint32> indices_;

	void generatePathfindingTerrain(const HeightMap& heightMap);
};

}

#endif /* PATHFINDINGTERRAIN_H_ */
