#ifndef ASREFOBJECT_H_
#define ASREFOBJECT_H_

#include "Types.hpp"

namespace hercules
{
namespace as_wrapper
{

class ASRefObject
{
public:
	ASRefObject();
	virtual ~ASRefObject();

	void addRef();
	void releaseRef();

protected:
	int32 refCount_;
};

}
}

#endif /* ASREFOBJECT_H_ */
