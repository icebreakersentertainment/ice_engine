#ifndef BINDABLE_H_
#define BINDABLE_H_

namespace hercules
{
namespace graphics
{
namespace custom
{
namespace gl
{

template <typename T>
class Bindable
{
public:
	void bind()
	{
		return static_cast<T*>(this)->bind();
	}
	
protected:
	~Bindable() = default;
};

}
}
}
}

#endif /* BINDABLE_H_ */

