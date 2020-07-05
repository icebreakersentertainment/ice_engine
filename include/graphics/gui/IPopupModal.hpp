#ifndef IPOPUPMODAL_H_
#define IPOPUPMODAL_H_

#include <string>
#include <functional>

#include "IComponent.hpp"
#include "IGenericComponentContainer.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IPopupModal : public virtual IGenericComponentContainer, public virtual IComponent
{
public:
	virtual ~IPopupModal()
	{
	}
	;

	virtual void close() = 0;

};

}
}
}

#endif /* IPOPUPMODAL_H_ */
