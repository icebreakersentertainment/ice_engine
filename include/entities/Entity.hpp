#ifndef ENTITY_H_
#define ENTITY_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace entities
{

class Entity : public handles::Handle<Entity>
{
	using handles::Handle<Entity>::Handle;
};

}
}

#endif /* ENTITY_H_ */
