#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "IComponent.hpp"
#include "ILabel.hpp"
#include "IButton.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

enum WindowFlags : uint32
{
	ICEENGINE_TITLE_BAR		= 1 << 0,
	ICEENGINE_SHOW_BORDERS	= 1 << 1,
	ICEENGINE_MOVABLE		= 1 << 2,
	ICEENGINE_NO_SCROLLBAR	= 1 << 3,
	ICEENGINE_CLOSABLE		= 1 << 4,
	ICEENGINE_MINIMIZABLE	= 1 << 5,
	ICEENGINE_RESIZABLE		= 1 << 6,
	ICEENGINE_NO_INPUT		= 1 << 7
};

class IWindow : public virtual IComponent
{
public:
	virtual ~IWindow()
	{
	}
	;
	
	virtual ILabel* createLabel(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string label = std::string()) = 0;
	virtual IButton* createButton(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string label = std::string()) = 0;
	
	virtual void setTitle(const std::string& title) = 0;
	virtual const std::string& getTitle() const  = 0;
	
	
};

}
}
}

#endif /* IWINDOW_H_ */
