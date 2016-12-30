#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include "entities/Entity.hpp"

namespace game
{
namespace entities
{

class EntityFactory
{
public:	
	static entities::Entity* entityRefFactory();

private:
	EntityFactory();
	EntityFactory(const EntityFactory& other);
	~EntityFactory();
};

}
}

#endif /* ENTITYFACTORY_H_ */
