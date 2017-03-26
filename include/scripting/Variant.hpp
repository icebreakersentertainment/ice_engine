#ifndef SCRIPT_VARIANT_H_
#define SCRIPT_VARIANT_H_

#include <functional>

namespace hercules
{
namespace scripting
{

template <typename T>
class Variant
{
public:
	Variant() {};
	virtual ~Variant() {};
	
	std::function<void(void*)> gen()
	{
		return [this](void* ptr) {
			value = *(T*)ptr;
		};
	};
	
	T value;
	
	/*
	Variant(int ok, std::function<void(void*)> copyFunction)
	{
		ok_ = ok;
		copyFunction_ = copyFunction;
	};
	
	
	
	int ok() { return ok_; };
	void set(void* ptr)
	{
		
	};
};

private:
	int ok_;
	std::function<void(void*)> copyFunction_;
*/
};

}
}

#endif /* SCRIPT_VARIANT_H_ */
