#ifndef ICOMPONENT_H_
#define ICOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/gui/Style.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IComponent
{
public:
	virtual ~IComponent() = default;

	virtual void render() = 0;
	virtual void tick(const float32 delta) = 0;

	virtual bool visible() const = 0;
	virtual void setVisible(const bool visible) = 0;

	virtual bool disabled() const = 0;
	virtual void setDisabled(const bool disabled) = 0;

	virtual void setStyle(const Style& style) = 0;
	virtual const Style& style() const = 0;

	virtual void setPosition(const uint32 x, const uint32 y) = 0;
	virtual glm::ivec2 position() const = 0;

	virtual void setDimensions(const uint32 with, const uint32 height) = 0;
	virtual glm::ivec2 dimensions() const  = 0;

	virtual void addComponent(IComponent* component) = 0;
	virtual void removeComponent(IComponent* component) = 0;
	virtual void removeAllComponents() = 0;
};

}
}
}

#endif /* ICOMPONENT_H_ */
