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
		graphics::TerrainHandle terrainHandle
	)
		: terrainHandle(terrainHandle)
	{
	};


	GraphicsTerrainComponent(
		graphics::TerrainHandle terrainHandle,
		graphics::TerrainRenderableHandle terrainRenderableHandle
	)
		: terrainRenderableHandle(terrainRenderableHandle),
		  terrainHandle(terrainHandle)
	{
	};
	
	static uint8 id()  { return 1; }

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
