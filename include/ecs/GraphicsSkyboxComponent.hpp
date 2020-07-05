#ifndef GRAPHICSSKYBOXCOMPONENT_H_
#define GRAPHICSSKYBOXCOMPONENT_H_

#include "graphics/SkyboxHandle.hpp"
#include "graphics/SkyboxRenderableHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct GraphicsSkyboxComponent
{
	GraphicsSkyboxComponent() = default;

	GraphicsSkyboxComponent(
		graphics::SkyboxHandle skyboxHandle
	)
		: skyboxHandle(skyboxHandle)
	{
	};


	GraphicsSkyboxComponent(
		graphics::SkyboxHandle skyboxHandle,
		graphics::SkyboxRenderableHandle skyboxRenderableHandle
	)
		: skyboxRenderableHandle(skyboxRenderableHandle),
		  skyboxHandle(skyboxHandle)
	{
	};

	static uint8 id()  { return 16; }

	graphics::SkyboxHandle skyboxHandle;
	graphics::SkyboxRenderableHandle skyboxRenderableHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::GraphicsSkyboxComponent& c, const unsigned int version)
{
	ar & c.skyboxHandle & c.skyboxRenderableHandle;
}

}
}

#endif /* GRAPHICSSKYBOXCOMPONENT_H_ */
