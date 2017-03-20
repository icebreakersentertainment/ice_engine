#ifndef ENTITY_H_
#define ENTITY_H_

#include "entities/ResourceHandle.hpp"

namespace hercules
{
namespace entities
{

class Entity : public ResourceHandle
{
	using ResourceHandle::ResourceHandle;
};

}
}

#endif /* ENTITY_H_ */
