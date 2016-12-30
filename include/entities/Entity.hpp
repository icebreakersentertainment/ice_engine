#ifndef ENTITY_H_
#define ENTITY_H_

#include "entities/GraphicsComponent.hpp"

namespace game
{
namespace entities
{

class Entity : public as_wrapper::ASRefObject {
public:
	Entity();
	Entity(entityx::Entity entity);
	virtual ~Entity();
	
	void addComponent(GraphicsComponent* graphicsComponent);
	void removeComponent(GraphicsComponent* graphicsComponent);
	
private:
	entityx::Entity entity_;
};

}
}

#endif /* ENTITY_H_ */
