#ifndef GRAPHICSCOMPONENT_H_
#define GRAPHICSCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ModelHandle.hpp"
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
	
	GraphicsComponent(ModelHandle&& modelHandle)
		: modelHandle(std::forward<ModelHandle>(modelHandle))
	{
	};

	GraphicsComponent(
		ModelHandle&& modelHandle,
		glm::vec3&& scale
	)
		: modelHandle(std::forward<ModelHandle>(modelHandle)),
		  scale(std::forward<glm::vec3>(scale))
	{
	};

	GraphicsComponent(
		ModelHandle&& modelHandle,
		glm::vec3&& scale,
		graphics::RenderableHandle&& renderableHandle
	)
		: modelHandle(std::forward<ModelHandle>(modelHandle)),
		  scale(std::forward<glm::vec3>(scale)),
		  renderableHandle(std::forward<graphics::RenderableHandle>(renderableHandle))
	{
	};
	
	ModelHandle modelHandle;
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
	ar & c.modelHandle & c.scale & c.renderableHandle;
}

}
}

#endif /* GRAPHICSCOMPONENT_H_ */
