#ifndef ASREFOBJECT_H_
#define ASREFOBJECT_H_

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
	int refCount_;
};

}
}

#endif /* ASREFOBJECT_H_ */
