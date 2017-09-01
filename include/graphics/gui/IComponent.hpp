#ifndef ICOMPONENT_H_
#define ICOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{
namespace gui
{

class IComponent
{
public:
	virtual ~IComponent()
	{
	}
	;
	
	virtual void render() = 0;
	virtual void tick(const float32 delta) = 0;
	
	virtual void setPosition(const uint32 x, const uint32 y) = 0;
	virtual glm::ivec2 getPosition() const = 0;
	
	virtual void setDimensions(const uint32 with, const uint32 height) = 0;
	virtual glm::ivec2 getDimensions() const  = 0;
	
	virtual void addComponent(IComponent* component) = 0;
	virtual void removeComponent(IComponent* component) = 0;
	virtual void removeAllComponents() = 0;
};

}
}
}

#endif /* ICOMPONENT_H_ */
