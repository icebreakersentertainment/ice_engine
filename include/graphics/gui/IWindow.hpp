#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "IComponent.hpp"
#include "IGenericComponentContainer.hpp"
#include "ILabel.hpp"
#include "IButton.hpp"
#include "IMenuBar.hpp"
#include "IRectangle.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

enum WindowFlags : uint32
{
	ICEENGINE_TITLE_BAR		= 1 << 0,
	ICEENGINE_MOVABLE		= 1 << 1,
	ICEENGINE_NO_SCROLLBAR	= 1 << 2,
	ICEENGINE_CLOSABLE		= 1 << 3,
	ICEENGINE_MINIMIZABLE	= 1 << 4,
	ICEENGINE_RESIZABLE		= 1 << 5,
	ICEENGINE_MENUBAR		= 1 << 6,
	ICEENGINE_NO_INPUT		= 1 << 7,
};

class IWindow : public virtual IComponent, public virtual IGenericComponentContainer
{
public:
	virtual ~IWindow()
	{
	}
	;

	// fix issue with other destroy functions not being seen
	using IGenericComponentContainer::destroy;

	virtual IMenuBar* createMenuBar() = 0;
	virtual IRectangle* createRectangle(const glm::vec2& start, const glm::vec2& end, const Color& color) = 0;

	virtual void destroy(const IRectangle* rectangle) = 0;

	virtual void setTitle(const std::string& title) = 0;
	virtual const std::string& getTitle() const  = 0;

	virtual void setBackgroundAlpha(const float32 alpha) = 0;
};

}
}
}

#endif /* IWINDOW_H_ */
