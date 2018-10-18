#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/MeshHandle.hpp"
#include "graphics/TextureHandle.hpp"
#include "graphics/RenderableHandle.hpp"

#include "serialization/glm/Vec3.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct GraphicsComponent
{
	GraphicsComponent() = default;
	
	GraphicsComponent(graphics::MeshHandle meshHandle)
		: meshHandle(meshHandle)
	{
	};

	GraphicsComponent(graphics::MeshHandle meshHandle, graphics::TextureHandle textureHandle)
		: meshHandle(meshHandle),
		  textureHandle(textureHandle)
	{
	};

	GraphicsComponent(
		graphics::MeshHandle meshHandle,
		graphics::TextureHandle textureHandle,
		glm::vec3 scale
	)
		: meshHandle(meshHandle),
		  textureHandle(textureHandle),
		  scale(scale)
	{
	};

	GraphicsComponent(
		graphics::MeshHandle meshHandle,
		graphics::TextureHandle textureHandle,
		glm::vec3 scale,
		graphics::RenderableHandle renderableHandle
	)
		: meshHandle(meshHandle),
		  textureHandle(textureHandle),
		  scale(scale),
		  renderableHandle(renderableHandle)
	{
	};
	
	static uint8 id()  { return 0; }

	graphics::MeshHandle meshHandle;
	graphics::TextureHandle textureHandle;
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	graphics::RenderableHandle renderableHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::GraphicsComponent& c, const unsigned int version)
{
	ar & c.meshHandle & c.textureHandle & c.scale & c.renderableHandle;
}

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
