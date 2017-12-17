#ifndef ILABEL_H_
#define ILABEL_H_

#include <string>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class ILabel : public virtual IComponent
{
public:
	virtual ~ILabel()
	{
	}
	;
	
	virtual void setLabel(const std::string& label) = 0;
	virtual const std::string& getLabel() const  = 0;
};

}
}
}

#endif /* ILABEL_H_ */
