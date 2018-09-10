#ifndef PATHFINDINGTERRAIN_H_
#define PATHFINDINGTERRAIN_H_

#include "pathfinding/ITerrain.hpp"

#include "HeightMap.hpp"

#include "detail/GenerateVertices.hpp"

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
	
	virtual ~PathfindingTerrain() = default;

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

	void generatePathfindingTerrain(const HeightMap& heightMap)
	{
		std::tie(vertices_, indices_) = detail::generateGrid(256);

		for (auto& v : vertices_ )
		{
			v.y = (((float32)(heightMap.height((uint32)v.x, (uint32)v.z)) / 255.0f) * 15.0f) - 7.5f;
			v.x = v.x - 128.0f;
			v.z = v.z - 128.0f;
		}

		for (int i=1; i < indices_.size()-1; i+=3)
		{
			uint32 temp = indices_[i-1];
			indices_[i-1] = indices_[i+1];
			indices_[i+1] = temp;
		}
	}
};

}

#endif /* PATHFINDINGTERRAIN_H_ */
