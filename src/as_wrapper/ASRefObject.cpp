#include "as_wrapper/ASRefObject.hpp"

namespace hercules
{
namespace as_wrapper
{

ASRefObject::ASRefObject()
{
	refCount_ = 1;
}

ASRefObject::~ASRefObject()
{
}

void ASRefObject::addRef()
{
	// increase the reference counter
	refCount_++;
}

void ASRefObject::releaseRef()
{
	// decrease reference counter and delete if it reaches 0
	if ( --refCount_ == 0 )
		delete this;
}

}
}
