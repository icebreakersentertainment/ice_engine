#ifndef IGUI_H_
#define IGUI_H_

#include <string>

#include "graphics/Event.hpp"

#include "IWindow.hpp"

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
	virtual ~IGui()
	{
	}
	;
	
	virtual void setViewport(const uint32 width, const uint32 height) = 0;
	
	virtual void render() = 0;
	virtual void tick(const float32 delta) = 0;
	
	virtual IWindow* createWindow(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const std::string title = std::string()) = 0;
	virtual IWindow* createWindow(const uint32 x, const uint32 y, const uint32 width, const uint32 height, const uint32 flags, const std::string title = std::string()) = 0;
	
	virtual bool processEvent(const graphics::WindowEvent& event) = 0;
	virtual bool processEvent(const graphics::KeyboardEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseButtonEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseMotionEvent& event) = 0;
	virtual bool processEvent(const graphics::MouseWheelEvent& event) = 0;
};

}
}
}

#endif /* IGUI_H_ */
