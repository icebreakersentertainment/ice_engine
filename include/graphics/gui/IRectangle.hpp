#ifndef IRECTANGLE_H_
#define IRECTANGLE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IComponent.hpp"

#include "graphics/Color.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IRectangle : public virtual IComponent
{
public:
	virtual ~IRectangle() = default;
	
	virtual void setPoints(const glm::vec2& start, const glm::vec2& end) = 0;
	virtual const glm::vec2& getStartPoint() const  = 0;
	virtual const glm::vec2& getEndPoint() const  = 0;

	virtual void setColor(const Color& color) = 0;
	virtual const Color& getColor() const  = 0;
};

}
}
}

#endif /* IRECTANGLE_H_ */
