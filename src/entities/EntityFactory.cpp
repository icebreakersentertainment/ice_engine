#include "entities/EntityFactory.hpp"

namespace hercules
{
namespace entities
{

EntityFactory::EntityFactory()
{
}

EntityFactory::EntityFactory(const EntityFactory& other)
{
	// TODO: Do I need an exception?  Do I realy need a full on class for this?
	// TODO: Exception
	//assert(0);
}

EntityFactory::~EntityFactory()
{
}

Entity* EntityFactory::entityRefFactory()
{
	return new Entity();
}

}
}
