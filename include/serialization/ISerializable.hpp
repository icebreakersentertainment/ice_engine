#ifndef ISERIALIZABLE_H_
#define ISERIALIZABLE_H_

#include <string>

namespace ice_engine
{
namespace serialization
{

/**
 * 
 */
class ISerializable
{
public:
	virtual ~ISerializable()
	{
	}
	;

	/**
	 * Serialize the object to the provided filename.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment.  When you are calling this
	 * method, no other threads should be accessing this object.
	 */
	virtual void serialize(const std::string& filename) = 0;
	
	/**
	 * Deserializes the object from the provided filename.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment.  When you are calling this
	 * method, no other threads should be accessing this object.
	 */
	virtual void deserialize(const std::string& filename) = 0;
};

}
}

#endif /* ISERIALIZABLE_H_ */
