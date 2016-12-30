#include "angel_script/ClassFactory.hpp"

namespace angel_script
{

/**
 * This is not an instantiable class.
 */
ClassFactory::ClassFactory()
{
}

ClassFactory::~ClassFactory()
{
}

ClassFactory::ClassFactory(const ClassFactory& other)
{
}

angel_script::TestAtom* ClassFactory::atomRefFactory()
{
	return new angel_script::TestAtom();
}

}
