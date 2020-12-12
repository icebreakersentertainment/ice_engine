#ifndef IGUI_H_
#define IGUI_H_

#include <string>

#include "graphics/Event.hpp"

#include "IWindow.hpp"
#include "IMainMenuBar.hpp"
#include "IPopupModal.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IGui
{
public:
	virtual ~IGui() = default;

	virtual void setViewport(const uint32 width, const uint32 height) = 0;

	virtual void render() = 0;
	virtual void tick(const float32 delta) = 0;

	virtual bool visible() const = 0;
	virtual void setVisible(const bool visible) = 0;

	virtual void setStyle(const Style& style) = 0;
	virtual const Style& style() const = 0;

	virtual IWindow* createWindow(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string title = std::string()) = 0;
	virtual IWindow* createWindow(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const uint32 flags, const std::string title = std::string()) = 0;

	virtual IMainMenuBar* createMainMenuBar() = 0;
	virtual void destroyMainMenuBar() = 0;

	virtual IPopupModal* createPopupModal(const std::string& title) = 0;
	virtual void destroy(const IWindow* window) = 0;
	virtual void destroy(const IPopupModal* popupModal) = 0;

	virtual bool processEvent(const graphics::WindowEvent& event) = 0;
	virtual bool processEvent(const graphics::KeyboardEvent& event) = 0;
    virtual bool processEvent(const graphics::TextInputEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseButtonEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseMotionEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseWheelEvent& event) = 0;
};

}
}
}

#endif /* IGUI_H_ */
