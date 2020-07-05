#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "PathfindingTerrain.hpp"

#include "detail/GenerateVertices.hpp"

namespace ice_engine
{

void PathfindingTerrain::generatePathfindingTerrain(const HeightMap& heightMap)
{
    std::tie(vertices_, indices_) = detail::generateGrid(heightMap.image()->width(), heightMap.image()->height());

    for (auto& v : vertices_ )
    {
        v.y = (((float32)(heightMap.height((uint32)v.x, (uint32)v.z)) / 255.0f) * 15.0f) - 7.5f;
        v.x = v.x - (float32)heightMap.image()->width() / 2.0f;
        v.z = v.z - (float32)heightMap.image()->height() / 2.0f;
    }

    for (int i=1; i < indices_.size()-1; i+=3)
    {
        uint32 temp = indices_[i-1];
        indices_[i-1] = indices_[i+1];
        indices_[i+1] = temp;
    }
}

}