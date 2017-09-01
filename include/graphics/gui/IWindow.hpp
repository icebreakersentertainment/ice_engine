#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "IComponent.hpp"
#include "ILabel.hpp"
#include "IButton.hpp"

namespace hercules
{
namespace graphics
{
namespace gui
{

enum WindowFlags : uint32
{
	HERCULES_TITLE_BAR		= 1 << 0,
	HERCULES_SHOW_BORDERS	= 1 << 1,
	HERCULES_MOVABLE		= 1 << 2,
	HERCULES_NO_SCROLLBAR	= 1 << 3,
	HERCULES_CLOSABLE		= 1 << 4,
	HERCULES_MINIMIZABLE	= 1 << 5,
	HERCULES_RESIZABLE		= 1 << 6
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
