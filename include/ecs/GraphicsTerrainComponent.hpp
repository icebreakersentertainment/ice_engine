#ifndef GRAPHICSTERRAINCOMPONENT_H_
#define GRAPHICSTERRAINCOMPONENT_H_

#include "graphics/TerrainHandle.hpp"
#include "graphics/TerrainRenderableHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct GraphicsTerrainComponent
{
	GraphicsTerrainComponent() = default;
	
	GraphicsTerrainComponent(
		graphics::TerrainHandle&& terrainHandle
	)
		: terrainHandle(std::forward<graphics::TerrainHandle>(terrainHandle))
	{
	};


	GraphicsTerrainComponent(
		graphics::TerrainHandle&& terrainHandle,
		graphics::TerrainRenderableHandle&& terrainRenderableHandle
	)
		: terrainRenderableHandle(std::forward<graphics::TerrainRenderableHandle>(terrainRenderableHandle)),
		  terrainHandle(std::forward<graphics::TerrainHandle>(terrainHandle))
	{
	};
	
	graphics::TerrainHandle terrainHandle;
	graphics::TerrainRenderableHandle terrainRenderableHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::GraphicsTerrainComponent& c, const unsigned int version)
{
	ar & c.terrainHandle & c.terrainRenderableHandle;
}

}
}

#endif /* GRAPHICSTERRAINCOMPONENT_H_ */
