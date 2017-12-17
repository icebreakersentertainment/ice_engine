#ifndef SCRIPT_RETURN_OBJECT_H_
#define SCRIPT_RETURN_OBJECT_H_

#include <functional>
#include <type_traits>

namespace ice_engine
{
namespace scripting
{

template <typename T>
class ReturnObject
{
// Only allowed for non-integral types
//static_assert(std::is_integral<T>::value, "T must be a non-integral type");
public:
	ReturnObject() {};
	virtual ~ReturnObject() {};
	
	std::function<void(void*)> parser()
	{
		return [this](void* ptr) {
			// The return value must be copied
			value = *(T*)ptr;
		};
	};
	
	T value;
};

}
}

#endif /* SCRIPT_RETURN_OBJECT_H_ */
