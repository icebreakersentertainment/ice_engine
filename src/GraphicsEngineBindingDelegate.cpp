#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "Scene.hpp"
#include "IWindowEventListener.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "GraphicsEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{	

template<class T>
void proxyFunctionSetPosition(const uint32 x, const uint32 y, T* t)
{
	t->setPosition(x, y);
}
template<class T>
glm::ivec2 proxyFunctionGetPosition(T* t)
{
	return t->getPosition();
}

template<class T>
void proxyFunctionSetDimensions(const uint32 x, const uint32 y, T* t)
{
	t->setDimensions(x, y);
}
template<class T>
glm::ivec2 proxyFunctionGetDimensions(T* t)
{
	return t->getDimensions();
}

template<class T>
void proxyFunctionSetStyle(const graphics::gui::Style& style, T* t)
{
	t->setStyle(style);
}
template<class T>
const graphics::gui::Style& proxyFunctionGetStyle(T* t)
{
	return t->getStyle();
}

static void InitConstructorGraphicsColor(graphics::Color* memory, const uint32 color) { new(memory) graphics::Color(color); }
static void InitConstructorGraphicsColor(graphics::Color* memory, const uint8 r, const uint8 g, const uint8 b, const uint8 a) { new(memory) graphics::Color(r, g, b, a); }

GraphicsEngineBindingDelegate::GraphicsEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	graphicsEngine_(graphicsEngine)
{
}

void GraphicsEngineBindingDelegate::bind()
{
	scriptingEngine_->registerObjectType("IImage", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("IHeightMap", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("ISplatMap", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("IDisplacementMap", 0, asOBJ_REF | asOBJ_NOCOUNT);

	// Enums available
	scriptingEngine_->registerEnum("TransformSpace");
	scriptingEngine_->registerEnumValue("TransformSpace", "TS_LOCAL", graphics::TransformSpace::TS_LOCAL);
	scriptingEngine_->registerEnumValue("TransformSpace", "TS_WORLD", graphics::TransformSpace::TS_WORLD);
	
	scriptingEngine_->registerEnum("State");
	scriptingEngine_->registerEnumValue("State", "RELEASED", graphics::RELEASED);
	scriptingEngine_->registerEnumValue("State", "PRESSED", graphics::PRESSED);
	
	scriptingEngine_->registerEnum("EventType");
	scriptingEngine_->registerEnumValue("EventType", "UNKNOWN", graphics::UNKNOWN);
	scriptingEngine_->registerEnumValue("EventType", "QUIT", graphics::QUIT);
	scriptingEngine_->registerEnumValue("EventType", "WINDOWEVENT", graphics::WINDOWEVENT);
	scriptingEngine_->registerEnumValue("EventType", "KEYDOWN", graphics::KEYDOWN);
	scriptingEngine_->registerEnumValue("EventType", "KEYUP", graphics::KEYUP);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEMOTION", graphics::MOUSEMOTION);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEBUTTONDOWN", graphics::MOUSEBUTTONDOWN);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEBUTTONUP", graphics::MOUSEBUTTONUP);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEWHEEL", graphics::MOUSEWHEEL);
	
	scriptingEngine_->registerEnum("WindowEventType");
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_UNKNOWN", graphics::WINDOWEVENT_UNKNOWN);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_NONE", graphics::WINDOWEVENT_NONE);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_SHOWN", graphics::WINDOWEVENT_SHOWN);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_HIDDEN", graphics::WINDOWEVENT_HIDDEN);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_EXPOSED", graphics::WINDOWEVENT_EXPOSED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_MOVED", graphics::WINDOWEVENT_MOVED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_RESIZED", graphics::WINDOWEVENT_RESIZED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_SIZE_CHANGED", graphics::WINDOWEVENT_SIZE_CHANGED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_MINIMIZED", graphics::WINDOWEVENT_MINIMIZED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_MAXIMIZED", graphics::WINDOWEVENT_MAXIMIZED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_RESTORED", graphics::WINDOWEVENT_RESTORED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_ENTER", graphics::WINDOWEVENT_ENTER);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_LEAVE", graphics::WINDOWEVENT_LEAVE);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_FOCUS_GAINED", graphics::WINDOWEVENT_FOCUS_GAINED);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_FOCUS_LOST", graphics::WINDOWEVENT_FOCUS_LOST);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_CLOSE", graphics::WINDOWEVENT_CLOSE);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_TAKE_FOCUS", graphics::WINDOWEVENT_TAKE_FOCUS);
	scriptingEngine_->registerEnumValue("WindowEventType", "WINDOWEVENT_HIT_TEST", graphics::WINDOWEVENT_HIT_TEST);
    
	
	scriptingEngine_->registerEnum("ScanCode");
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_UNKNOWN", graphics::SCANCODE_UNKNOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_A", graphics::SCANCODE_A);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_B", graphics::SCANCODE_B);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_C", graphics::SCANCODE_C);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_D", graphics::SCANCODE_D);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_E", graphics::SCANCODE_E);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F", graphics::SCANCODE_F);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_G", graphics::SCANCODE_G);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_H", graphics::SCANCODE_H);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_I", graphics::SCANCODE_I);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_J", graphics::SCANCODE_J);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_K", graphics::SCANCODE_K);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_L", graphics::SCANCODE_L);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_M", graphics::SCANCODE_M);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_N", graphics::SCANCODE_N);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_O", graphics::SCANCODE_O);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_P", graphics::SCANCODE_P);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_Q", graphics::SCANCODE_Q);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_R", graphics::SCANCODE_R);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_S", graphics::SCANCODE_S);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_T", graphics::SCANCODE_T);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_U", graphics::SCANCODE_U);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_V", graphics::SCANCODE_V);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_W", graphics::SCANCODE_W);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_X", graphics::SCANCODE_X);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_Y", graphics::SCANCODE_Y);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_Z", graphics::SCANCODE_Z);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_1", graphics::SCANCODE_1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_2", graphics::SCANCODE_2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_3", graphics::SCANCODE_3);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_4", graphics::SCANCODE_4);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_5", graphics::SCANCODE_5);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_6", graphics::SCANCODE_6);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_7", graphics::SCANCODE_7);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_8", graphics::SCANCODE_8);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_9", graphics::SCANCODE_9);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_0", graphics::SCANCODE_0);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RETURN", graphics::SCANCODE_RETURN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_ESCAPE", graphics::SCANCODE_ESCAPE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_BACKSPACE", graphics::SCANCODE_BACKSPACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_TAB", graphics::SCANCODE_TAB);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SPACE", graphics::SCANCODE_SPACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MINUS", graphics::SCANCODE_MINUS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_EQUALS", graphics::SCANCODE_EQUALS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LEFTBRACKET", graphics::SCANCODE_LEFTBRACKET);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RIGHTBRACKET", graphics::SCANCODE_RIGHTBRACKET);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_BACKSLASH", graphics::SCANCODE_BACKSLASH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_NONUSHASH", graphics::SCANCODE_NONUSHASH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SEMICOLON", graphics::SCANCODE_SEMICOLON);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_APOSTROPHE", graphics::SCANCODE_APOSTROPHE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_GRAVE", graphics::SCANCODE_GRAVE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_COMMA", graphics::SCANCODE_COMMA);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PERIOD", graphics::SCANCODE_PERIOD);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SLASH", graphics::SCANCODE_SLASH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CAPSLOCK", graphics::SCANCODE_CAPSLOCK);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F1", graphics::SCANCODE_F1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F2", graphics::SCANCODE_F2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F3", graphics::SCANCODE_F3);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F4", graphics::SCANCODE_F4);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F5", graphics::SCANCODE_F5);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F6", graphics::SCANCODE_F6);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F7", graphics::SCANCODE_F7);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F8", graphics::SCANCODE_F8);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F9", graphics::SCANCODE_F9);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F10", graphics::SCANCODE_F10);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F11", graphics::SCANCODE_F11);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F12", graphics::SCANCODE_F12);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PRINTSCREEN", graphics::SCANCODE_PRINTSCREEN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SCROLLLOCK", graphics::SCANCODE_SCROLLLOCK);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PAUSE", graphics::SCANCODE_PAUSE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INSERT", graphics::SCANCODE_INSERT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_HOME", graphics::SCANCODE_HOME);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PAGEUP", graphics::SCANCODE_PAGEUP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_DELETE", graphics::SCANCODE_DELETE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_END", graphics::SCANCODE_END);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PAGEDOWN", graphics::SCANCODE_PAGEDOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RIGHT", graphics::SCANCODE_RIGHT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LEFT", graphics::SCANCODE_LEFT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_DOWN", graphics::SCANCODE_DOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_UP", graphics::SCANCODE_UP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_NUMLOCKCLEAR", graphics::SCANCODE_NUMLOCKCLEAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_DIVIDE", graphics::SCANCODE_KP_DIVIDE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MULTIPLY", graphics::SCANCODE_KP_MULTIPLY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MINUS", graphics::SCANCODE_KP_MINUS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_PLUS", graphics::SCANCODE_KP_PLUS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_ENTER", graphics::SCANCODE_KP_ENTER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_1", graphics::SCANCODE_KP_1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_2", graphics::SCANCODE_KP_2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_3", graphics::SCANCODE_KP_3);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_4", graphics::SCANCODE_KP_4);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_5", graphics::SCANCODE_KP_5);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_6", graphics::SCANCODE_KP_6);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_7", graphics::SCANCODE_KP_7);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_8", graphics::SCANCODE_KP_8);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_9", graphics::SCANCODE_KP_9);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_0", graphics::SCANCODE_KP_0);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_PERIOD", graphics::SCANCODE_KP_PERIOD);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_NONUSBACKSLASH", graphics::SCANCODE_NONUSBACKSLASH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_APPLICATION", graphics::SCANCODE_APPLICATION);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_POWER", graphics::SCANCODE_POWER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_EQUALS", graphics::SCANCODE_KP_EQUALS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F13", graphics::SCANCODE_F13);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F14", graphics::SCANCODE_F14);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F15", graphics::SCANCODE_F15);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F16", graphics::SCANCODE_F16);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F17", graphics::SCANCODE_F17);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F18", graphics::SCANCODE_F18);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F19", graphics::SCANCODE_F19);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F20", graphics::SCANCODE_F20);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F21", graphics::SCANCODE_F21);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F22", graphics::SCANCODE_F22);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F23", graphics::SCANCODE_F23);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_F24", graphics::SCANCODE_F24);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_EXECUTE", graphics::SCANCODE_EXECUTE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_HELP", graphics::SCANCODE_HELP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MENU", graphics::SCANCODE_MENU);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SELECT", graphics::SCANCODE_SELECT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_STOP", graphics::SCANCODE_STOP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AGAIN", graphics::SCANCODE_AGAIN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_UNDO", graphics::SCANCODE_UNDO);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CUT", graphics::SCANCODE_CUT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_COPY", graphics::SCANCODE_COPY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PASTE", graphics::SCANCODE_PASTE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_FIND", graphics::SCANCODE_FIND);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MUTE", graphics::SCANCODE_MUTE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_VOLUMEUP", graphics::SCANCODE_VOLUMEUP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_VOLUMEDOWN", graphics::SCANCODE_VOLUMEDOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_COMMA", graphics::SCANCODE_KP_COMMA);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_EQUALSAS400", graphics::SCANCODE_KP_EQUALSAS400);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL1", graphics::SCANCODE_INTERNATIONAL1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL2", graphics::SCANCODE_INTERNATIONAL2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL3", graphics::SCANCODE_INTERNATIONAL3);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL4", graphics::SCANCODE_INTERNATIONAL4);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL5", graphics::SCANCODE_INTERNATIONAL5);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL6", graphics::SCANCODE_INTERNATIONAL6);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL7", graphics::SCANCODE_INTERNATIONAL7);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL8", graphics::SCANCODE_INTERNATIONAL8);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_INTERNATIONAL9", graphics::SCANCODE_INTERNATIONAL9);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG1", graphics::SCANCODE_LANG1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG2", graphics::SCANCODE_LANG2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG3", graphics::SCANCODE_LANG3);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG4", graphics::SCANCODE_LANG4);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG5", graphics::SCANCODE_LANG5);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG6", graphics::SCANCODE_LANG6);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG7", graphics::SCANCODE_LANG7);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG8", graphics::SCANCODE_LANG8);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LANG9", graphics::SCANCODE_LANG9);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_ALTERASE", graphics::SCANCODE_ALTERASE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SYSREQ", graphics::SCANCODE_SYSREQ);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CANCEL", graphics::SCANCODE_CANCEL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CLEAR", graphics::SCANCODE_CLEAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_PRIOR", graphics::SCANCODE_PRIOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RETURN2", graphics::SCANCODE_RETURN2);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SEPARATOR", graphics::SCANCODE_SEPARATOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_OUT", graphics::SCANCODE_OUT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_OPER", graphics::SCANCODE_OPER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CLEARAGAIN", graphics::SCANCODE_CLEARAGAIN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CRSEL", graphics::SCANCODE_CRSEL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_EXSEL", graphics::SCANCODE_EXSEL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_00", graphics::SCANCODE_KP_00);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_000", graphics::SCANCODE_KP_000);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_THOUSANDSSEPARATOR", graphics::SCANCODE_THOUSANDSSEPARATOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_DECIMALSEPARATOR", graphics::SCANCODE_DECIMALSEPARATOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CURRENCYUNIT", graphics::SCANCODE_CURRENCYUNIT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CURRENCYSUBUNIT", graphics::SCANCODE_CURRENCYSUBUNIT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_LEFTPAREN", graphics::SCANCODE_KP_LEFTPAREN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_RIGHTPAREN", graphics::SCANCODE_KP_RIGHTPAREN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_LEFTBRACE", graphics::SCANCODE_KP_LEFTBRACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_RIGHTBRACE", graphics::SCANCODE_KP_RIGHTBRACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_TAB", graphics::SCANCODE_KP_TAB);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_BACKSPACE", graphics::SCANCODE_KP_BACKSPACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_A", graphics::SCANCODE_KP_A);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_B", graphics::SCANCODE_KP_B);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_C", graphics::SCANCODE_KP_C);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_D", graphics::SCANCODE_KP_D);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_E", graphics::SCANCODE_KP_E);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_F", graphics::SCANCODE_KP_F);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_XOR", graphics::SCANCODE_KP_XOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_POWER", graphics::SCANCODE_KP_POWER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_PERCENT", graphics::SCANCODE_KP_PERCENT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_LESS", graphics::SCANCODE_KP_LESS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_GREATER", graphics::SCANCODE_KP_GREATER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_AMPERSAND", graphics::SCANCODE_KP_AMPERSAND);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_DBLAMPERSAND", graphics::SCANCODE_KP_DBLAMPERSAND);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_VERTICALBAR", graphics::SCANCODE_KP_VERTICALBAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_DBLVERTICALBAR", graphics::SCANCODE_KP_DBLVERTICALBAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_COLON", graphics::SCANCODE_KP_COLON);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_HASH", graphics::SCANCODE_KP_HASH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_SPACE", graphics::SCANCODE_KP_SPACE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_AT", graphics::SCANCODE_KP_AT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_EXCLAM", graphics::SCANCODE_KP_EXCLAM);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMSTORE", graphics::SCANCODE_KP_MEMSTORE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMRECALL", graphics::SCANCODE_KP_MEMRECALL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMCLEAR", graphics::SCANCODE_KP_MEMCLEAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMADD", graphics::SCANCODE_KP_MEMADD);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMSUBTRACT", graphics::SCANCODE_KP_MEMSUBTRACT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMMULTIPLY", graphics::SCANCODE_KP_MEMMULTIPLY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_MEMDIVIDE", graphics::SCANCODE_KP_MEMDIVIDE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_PLUSMINUS", graphics::SCANCODE_KP_PLUSMINUS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_CLEAR", graphics::SCANCODE_KP_CLEAR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_CLEARENTRY", graphics::SCANCODE_KP_CLEARENTRY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_BINARY", graphics::SCANCODE_KP_BINARY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_OCTAL", graphics::SCANCODE_KP_OCTAL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_DECIMAL", graphics::SCANCODE_KP_DECIMAL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KP_HEXADECIMAL", graphics::SCANCODE_KP_HEXADECIMAL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LCTRL", graphics::SCANCODE_LCTRL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LSHIFT", graphics::SCANCODE_LSHIFT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LALT", graphics::SCANCODE_LALT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_LGUI", graphics::SCANCODE_LGUI);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RCTRL", graphics::SCANCODE_RCTRL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RSHIFT", graphics::SCANCODE_RSHIFT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RALT", graphics::SCANCODE_RALT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_RGUI", graphics::SCANCODE_RGUI);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MODE", graphics::SCANCODE_MODE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AUDIONEXT", graphics::SCANCODE_AUDIONEXT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AUDIOPREV", graphics::SCANCODE_AUDIOPREV);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AUDIOSTOP", graphics::SCANCODE_AUDIOSTOP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AUDIOPLAY", graphics::SCANCODE_AUDIOPLAY);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AUDIOMUTE", graphics::SCANCODE_AUDIOMUTE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MEDIASELECT", graphics::SCANCODE_MEDIASELECT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_WWW", graphics::SCANCODE_WWW);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_MAIL", graphics::SCANCODE_MAIL);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_CALCULATOR", graphics::SCANCODE_CALCULATOR);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_COMPUTER", graphics::SCANCODE_COMPUTER);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_SEARCH", graphics::SCANCODE_AC_SEARCH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_HOME", graphics::SCANCODE_AC_HOME);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_BACK", graphics::SCANCODE_AC_BACK);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_FORWARD", graphics::SCANCODE_AC_FORWARD);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_STOP", graphics::SCANCODE_AC_STOP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_REFRESH", graphics::SCANCODE_AC_REFRESH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_AC_BOOKMARKS", graphics::SCANCODE_AC_BOOKMARKS);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_BRIGHTNESSDOWN", graphics::SCANCODE_BRIGHTNESSDOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_BRIGHTNESSUP", graphics::SCANCODE_BRIGHTNESSUP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_DISPLAYSWITCH", graphics::SCANCODE_DISPLAYSWITCH);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KBDILLUMTOGGLE", graphics::SCANCODE_KBDILLUMTOGGLE);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KBDILLUMDOWN", graphics::SCANCODE_KBDILLUMDOWN);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_KBDILLUMUP", graphics::SCANCODE_KBDILLUMUP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_EJECT", graphics::SCANCODE_EJECT);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_SLEEP", graphics::SCANCODE_SLEEP);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_APP1", graphics::SCANCODE_APP1);
	scriptingEngine_->registerEnumValue("ScanCode", "SCANCODE_APP2", graphics::SCANCODE_APP2);
	
	scriptingEngine_->registerEnum("KeyCode");
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_UNKNOWN", graphics::KEY_UNKNOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RETURN", graphics::KEY_RETURN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_ESCAPE", graphics::KEY_ESCAPE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_BACKSPACE", graphics::KEY_BACKSPACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_TAB", graphics::KEY_TAB);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SPACE", graphics::KEY_SPACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_EXCLAIM", graphics::KEY_EXCLAIM);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_QUOTEDBL", graphics::KEY_QUOTEDBL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_HASH", graphics::KEY_HASH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PERCENT", graphics::KEY_PERCENT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_DOLLAR", graphics::KEY_DOLLAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AMPERSAND", graphics::KEY_AMPERSAND);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_QUOTE", graphics::KEY_QUOTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LEFTPAREN", graphics::KEY_LEFTPAREN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RIGHTPAREN", graphics::KEY_RIGHTPAREN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_ASTERISK", graphics::KEY_ASTERISK);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PLUS", graphics::KEY_PLUS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_COMMA", graphics::KEY_COMMA);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MINUS", graphics::KEY_MINUS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PERIOD", graphics::KEY_PERIOD);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SLASH", graphics::KEY_SLASH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_0", graphics::KEY_0);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_1", graphics::KEY_1);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_2", graphics::KEY_2);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_3", graphics::KEY_3);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_4", graphics::KEY_4);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_5", graphics::KEY_5);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_6", graphics::KEY_6);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_7", graphics::KEY_7);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_8", graphics::KEY_8);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_9", graphics::KEY_9);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_COLON", graphics::KEY_COLON);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SEMICOLON", graphics::KEY_SEMICOLON);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LESS", graphics::KEY_LESS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_EQUALS", graphics::KEY_EQUALS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_GREATER", graphics::KEY_GREATER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_QUESTION", graphics::KEY_QUESTION);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AT", graphics::KEY_AT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LEFTBRACKET", graphics::KEY_LEFTBRACKET);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_BACKSLASH", graphics::KEY_BACKSLASH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RIGHTBRACKET", graphics::KEY_RIGHTBRACKET);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CARET", graphics::KEY_CARET);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_UNDERSCORE", graphics::KEY_UNDERSCORE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_BACKQUOTE", graphics::KEY_BACKQUOTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_a", graphics::KEY_a);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_b", graphics::KEY_b);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_c", graphics::KEY_c);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_d", graphics::KEY_d);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_e", graphics::KEY_e);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_f", graphics::KEY_f);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_g", graphics::KEY_g);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_h", graphics::KEY_h);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_i", graphics::KEY_i);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_j", graphics::KEY_j);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_k", graphics::KEY_k);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_l", graphics::KEY_l);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_m", graphics::KEY_m);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_n", graphics::KEY_n);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_o", graphics::KEY_o);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_p", graphics::KEY_p);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_q", graphics::KEY_q);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_r", graphics::KEY_r);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_s", graphics::KEY_s);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_t", graphics::KEY_t);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_u", graphics::KEY_u);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_v", graphics::KEY_v);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_w", graphics::KEY_w);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_x", graphics::KEY_x);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_y", graphics::KEY_y);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_z", graphics::KEY_z);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CAPSLOCK", graphics::KEY_CAPSLOCK);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F1", graphics::KEY_F1);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F2", graphics::KEY_F2);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F3", graphics::KEY_F3);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F4", graphics::KEY_F4);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F5", graphics::KEY_F5);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F6", graphics::KEY_F6);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F7", graphics::KEY_F7);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F8", graphics::KEY_F8);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F9", graphics::KEY_F9);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F10", graphics::KEY_F10);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F11", graphics::KEY_F11);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F12", graphics::KEY_F12);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PRINTSCREEN", graphics::KEY_PRINTSCREEN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SCROLLLOCK", graphics::KEY_SCROLLLOCK);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PAUSE", graphics::KEY_PAUSE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_INSERT", graphics::KEY_INSERT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_HOME", graphics::KEY_HOME);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PAGEUP", graphics::KEY_PAGEUP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_DELETE", graphics::KEY_DELETE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_END", graphics::KEY_END);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PAGEDOWN", graphics::KEY_PAGEDOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RIGHT", graphics::KEY_RIGHT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LEFT", graphics::KEY_LEFT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_DOWN", graphics::KEY_DOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_UP", graphics::KEY_UP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_NUMLOCKCLEAR", graphics::KEY_NUMLOCKCLEAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_DIVIDE", graphics::KEY_KP_DIVIDE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MULTIPLY", graphics::KEY_KP_MULTIPLY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MINUS", graphics::KEY_KP_MINUS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_PLUS", graphics::KEY_KP_PLUS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_ENTER", graphics::KEY_KP_ENTER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_1", graphics::KEY_KP_1);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_2", graphics::KEY_KP_2);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_3", graphics::KEY_KP_3);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_4", graphics::KEY_KP_4);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_5", graphics::KEY_KP_5);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_6", graphics::KEY_KP_6);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_7", graphics::KEY_KP_7);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_8", graphics::KEY_KP_8);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_9", graphics::KEY_KP_9);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_0", graphics::KEY_KP_0);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_PERIOD", graphics::KEY_KP_PERIOD);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_APPLICATION", graphics::KEY_APPLICATION);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_POWER", graphics::KEY_POWER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_EQUALS", graphics::KEY_KP_EQUALS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F13", graphics::KEY_F13);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F14", graphics::KEY_F14);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F15", graphics::KEY_F15);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F16", graphics::KEY_F16);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F17", graphics::KEY_F17);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F18", graphics::KEY_F18);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F19", graphics::KEY_F19);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F20", graphics::KEY_F20);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F21", graphics::KEY_F21);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F22", graphics::KEY_F22);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F23", graphics::KEY_F23);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_F24", graphics::KEY_F24);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_EXECUTE", graphics::KEY_EXECUTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_HELP", graphics::KEY_HELP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MENU", graphics::KEY_MENU);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SELECT", graphics::KEY_SELECT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_STOP", graphics::KEY_STOP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AGAIN", graphics::KEY_AGAIN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_UNDO", graphics::KEY_UNDO);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CUT", graphics::KEY_CUT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_COPY", graphics::KEY_COPY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PASTE", graphics::KEY_PASTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_FIND", graphics::KEY_FIND);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MUTE", graphics::KEY_MUTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_VOLUMEUP", graphics::KEY_VOLUMEUP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_VOLUMEDOWN", graphics::KEY_VOLUMEDOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_COMMA", graphics::KEY_KP_COMMA);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_EQUALSAS400", graphics::KEY_KP_EQUALSAS400);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_ALTERASE", graphics::KEY_ALTERASE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SYSREQ", graphics::KEY_SYSREQ);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CANCEL", graphics::KEY_CANCEL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CLEAR", graphics::KEY_CLEAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_PRIOR", graphics::KEY_PRIOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RETURN2", graphics::KEY_RETURN2);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SEPARATOR", graphics::KEY_SEPARATOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_OUT", graphics::KEY_OUT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_OPER", graphics::KEY_OPER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CLEARAGAIN", graphics::KEY_CLEARAGAIN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CRSEL", graphics::KEY_CRSEL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_EXSEL", graphics::KEY_EXSEL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_00", graphics::KEY_KP_00);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_000", graphics::KEY_KP_000);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_THOUSANDSSEPARATOR", graphics::KEY_THOUSANDSSEPARATOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_DECIMALSEPARATOR", graphics::KEY_DECIMALSEPARATOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CURRENCYUNIT", graphics::KEY_CURRENCYUNIT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CURRENCYSUBUNIT", graphics::KEY_CURRENCYSUBUNIT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_LEFTPAREN", graphics::KEY_KP_LEFTPAREN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_RIGHTPAREN", graphics::KEY_KP_RIGHTPAREN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_LEFTBRACE", graphics::KEY_KP_LEFTBRACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_RIGHTBRACE", graphics::KEY_KP_RIGHTBRACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_TAB", graphics::KEY_KP_TAB);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_BACKSPACE", graphics::KEY_KP_BACKSPACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_A", graphics::KEY_KP_A);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_B", graphics::KEY_KP_B);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_C", graphics::KEY_KP_C);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_D", graphics::KEY_KP_D);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_E", graphics::KEY_KP_E);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_F", graphics::KEY_KP_F);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_XOR", graphics::KEY_KP_XOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_POWER", graphics::KEY_KP_POWER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_PERCENT", graphics::KEY_KP_PERCENT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_LESS", graphics::KEY_KP_LESS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_GREATER", graphics::KEY_KP_GREATER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_AMPERSAND", graphics::KEY_KP_AMPERSAND);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_DBLAMPERSAND", graphics::KEY_KP_DBLAMPERSAND);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_VERTICALBAR", graphics::KEY_KP_VERTICALBAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_DBLVERTICALBAR", graphics::KEY_KP_DBLVERTICALBAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_COLON", graphics::KEY_KP_COLON);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_HASH", graphics::KEY_KP_HASH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_SPACE", graphics::KEY_KP_SPACE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_AT", graphics::KEY_KP_AT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_EXCLAM", graphics::KEY_KP_EXCLAM);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMSTORE", graphics::KEY_KP_MEMSTORE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMRECALL", graphics::KEY_KP_MEMRECALL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMCLEAR", graphics::KEY_KP_MEMCLEAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMADD", graphics::KEY_KP_MEMADD);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMSUBTRACT", graphics::KEY_KP_MEMSUBTRACT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMMULTIPLY", graphics::KEY_KP_MEMMULTIPLY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_MEMDIVIDE", graphics::KEY_KP_MEMDIVIDE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_PLUSMINUS", graphics::KEY_KP_PLUSMINUS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_CLEAR", graphics::KEY_KP_CLEAR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_CLEARENTRY", graphics::KEY_KP_CLEARENTRY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_BINARY", graphics::KEY_KP_BINARY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_OCTAL", graphics::KEY_KP_OCTAL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_DECIMAL", graphics::KEY_KP_DECIMAL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KP_HEXADECIMAL", graphics::KEY_KP_HEXADECIMAL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LCTRL", graphics::KEY_LCTRL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LSHIFT", graphics::KEY_LSHIFT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LALT", graphics::KEY_LALT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_LGUI", graphics::KEY_LGUI);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RCTRL", graphics::KEY_RCTRL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RSHIFT", graphics::KEY_RSHIFT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RALT", graphics::KEY_RALT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_RGUI", graphics::KEY_RGUI);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MODE", graphics::KEY_MODE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AUDIONEXT", graphics::KEY_AUDIONEXT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AUDIOPREV", graphics::KEY_AUDIOPREV);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AUDIOSTOP", graphics::KEY_AUDIOSTOP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AUDIOPLAY", graphics::KEY_AUDIOPLAY);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AUDIOMUTE", graphics::KEY_AUDIOMUTE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MEDIASELECT", graphics::KEY_MEDIASELECT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_WWW", graphics::KEY_WWW);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_MAIL", graphics::KEY_MAIL);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_CALCULATOR", graphics::KEY_CALCULATOR);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_COMPUTER", graphics::KEY_COMPUTER);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_SEARCH", graphics::KEY_AC_SEARCH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_HOME", graphics::KEY_AC_HOME);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_BACK", graphics::KEY_AC_BACK);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_FORWARD", graphics::KEY_AC_FORWARD);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_STOP", graphics::KEY_AC_STOP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_REFRESH", graphics::KEY_AC_REFRESH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_AC_BOOKMARKS", graphics::KEY_AC_BOOKMARKS);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_BRIGHTNESSDOWN", graphics::KEY_BRIGHTNESSDOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_BRIGHTNESSUP", graphics::KEY_BRIGHTNESSUP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_DISPLAYSWITCH", graphics::KEY_DISPLAYSWITCH);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KBDILLUMTOGGLE", graphics::KEY_KBDILLUMTOGGLE);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KBDILLUMDOWN", graphics::KEY_KBDILLUMDOWN);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_KBDILLUMUP", graphics::KEY_KBDILLUMUP);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_EJECT", graphics::KEY_EJECT);
	scriptingEngine_->registerEnumValue("KeyCode", "KEY_SLEEP", graphics::KEY_SLEEP);
	
	scriptingEngine_->registerEnum("KeyState");
	scriptingEngine_->registerEnumValue("KeyState", "KEY_PRESSED", graphics::KEY_PRESSED);
	scriptingEngine_->registerEnumValue("KeyState", "KEY_RELEASED", graphics::KEY_RELEASED);
	
	scriptingEngine_->registerEnum("KeyMod");
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_NONE", graphics::KEYMOD_NONE);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_LSHIFT", graphics::KEYMOD_LSHIFT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_RSHIFT", graphics::KEYMOD_RSHIFT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_LCTRL", graphics::KEYMOD_LCTRL);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_RCTRL", graphics::KEYMOD_RCTRL);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_LALT", graphics::KEYMOD_LALT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_RALT", graphics::KEYMOD_RALT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_LGUI", graphics::KEYMOD_LGUI);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_RGUI", graphics::KEYMOD_RGUI);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_NUM", graphics::KEYMOD_NUM);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_CAPS", graphics::KEYMOD_CAPS);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_MODE", graphics::KEYMOD_MODE);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_RESERVED", graphics::KEYMOD_RESERVED);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_CTRL", graphics::KEYMOD_CTRL);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_SHIFT", graphics::KEYMOD_SHIFT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_ALT", graphics::KEYMOD_ALT);
	scriptingEngine_->registerEnumValue("KeyMod", "KEYMOD_GUI", graphics::KEYMOD_GUI);
	
	scriptingEngine_->registerEnum("MouseButtonCode");
	scriptingEngine_->registerEnumValue("MouseButtonCode", "BUTTON_LEFT", graphics::BUTTON_LEFT);
	scriptingEngine_->registerEnumValue("MouseButtonCode", "BUTTON_MIDDLE", graphics::BUTTON_MIDDLE);
	scriptingEngine_->registerEnumValue("MouseButtonCode", "BUTTON_RIGHT", graphics::BUTTON_RIGHT);
	scriptingEngine_->registerEnumValue("MouseButtonCode", "BUTTON_X1", graphics::BUTTON_X1);
	scriptingEngine_->registerEnumValue("MouseButtonCode", "BUTTON_X2", graphics::BUTTON_X2);
	
	scriptingEngine_->registerEnum("WindowFlags");
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_TITLE_BAR", graphics::gui::ICEENGINE_TITLE_BAR);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_MOVABLE", graphics::gui::ICEENGINE_MOVABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_NO_SCROLLBAR", graphics::gui::ICEENGINE_NO_SCROLLBAR);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_CLOSABLE", graphics::gui::ICEENGINE_CLOSABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_MINIMIZABLE", graphics::gui::ICEENGINE_MINIMIZABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_RESIZABLE", graphics::gui::ICEENGINE_RESIZABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_MENUBAR", graphics::gui::ICEENGINE_MENUBAR);
	scriptingEngine_->registerEnumValue("WindowFlags", "ICEENGINE_NO_INPUT", graphics::gui::ICEENGINE_NO_INPUT);
	
	registerHandleBindings<graphics::CameraHandle>(scriptingEngine_, "CameraHandle");
	registerHandleBindings<graphics::RenderSceneHandle>(scriptingEngine_, "RenderSceneHandle");
	registerHandleBindings<graphics::MeshHandle>(scriptingEngine_, "MeshHandle");
	registerHandleBindings<graphics::TextureHandle>(scriptingEngine_, "TextureHandle");
	registerHandleBindings<graphics::RenderableHandle>(scriptingEngine_, "RenderableHandle");
	registerHandleBindings<graphics::TerrainHandle>(scriptingEngine_, "TerrainHandle");
	registerHandleBindings<graphics::TerrainRenderableHandle>(scriptingEngine_, "TerrainRenderableHandle");
	registerHandleBindings<graphics::PointLightHandle>(scriptingEngine_, "PointLightHandle");
	registerHandleBindings<graphics::VertexShaderHandle>(scriptingEngine_, "VertexShaderHandle");
	registerHandleBindings<graphics::FragmentShaderHandle>(scriptingEngine_, "FragmentShaderHandle");
	registerHandleBindings<graphics::ShaderProgramHandle>(scriptingEngine_, "ShaderProgramHandle");
	
	//scriptingEngine_->registerObjectType("KeySym", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("KeySym", sizeof(graphics::KeySym), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::KeySym>());
	scriptingEngine_->registerObjectProperty("KeySym", "ScanCode scancode", asOFFSET(graphics::KeySym, scancode));
	scriptingEngine_->registerObjectProperty("KeySym", "KeyCode sym", asOFFSET(graphics::KeySym, sym));
	scriptingEngine_->registerObjectProperty("KeySym", "KeyMod mod", asOFFSET(graphics::KeySym, mod));
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmKeySym::DefaultConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f(const KeySym &in)", asFUNCTION(glmKeySym::CopyConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(glmKeySym::InitConstructor), asCALL_CDECL_OBJLAST);
	
	scriptingEngine_->registerObjectType("WindowEvent", sizeof(graphics::WindowEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::WindowEvent>());
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint32 type", asOFFSET(graphics::WindowEvent, type));
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint32 timestamp", asOFFSET(graphics::WindowEvent, timestamp));
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint32 windowId", asOFFSET(graphics::WindowEvent, windowId));
	scriptingEngine_->registerObjectProperty("WindowEvent", "WindowEventType eventType", asOFFSET(graphics::WindowEvent, eventType));
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint8 padding1", asOFFSET(graphics::WindowEvent, padding1));
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint8 padding2", asOFFSET(graphics::WindowEvent, padding2));
	scriptingEngine_->registerObjectProperty("WindowEvent", "uint8 padding3", asOFFSET(graphics::WindowEvent, padding3));
	scriptingEngine_->registerObjectProperty("WindowEvent", "int32 data1", asOFFSET(graphics::WindowEvent, data1));
	scriptingEngine_->registerObjectProperty("WindowEvent", "int32 data2", asOFFSET(graphics::WindowEvent, data2));
	scriptingEngine_->registerObjectType("KeyboardEvent", sizeof(graphics::KeyboardEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::KeyboardEvent>());
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "uint32 type", asOFFSET(graphics::KeyboardEvent, type));
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "uint8 state", asOFFSET(graphics::KeyboardEvent, state));
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "uint8 repeat", asOFFSET(graphics::KeyboardEvent, repeat));
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "uint8 padding2", asOFFSET(graphics::KeyboardEvent, padding2));
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "uint8 padding3", asOFFSET(graphics::KeyboardEvent, padding3));
	scriptingEngine_->registerObjectProperty("KeyboardEvent", "KeySym keySym", asOFFSET(graphics::KeyboardEvent, keySym));
	scriptingEngine_->registerObjectType("MouseMotionEvent", sizeof(graphics::MouseMotionEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::MouseMotionEvent>());
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "uint32 type", asOFFSET(graphics::MouseMotionEvent, type));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "uint8 state", asOFFSET(graphics::MouseMotionEvent, state));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "uint8 padding1", asOFFSET(graphics::MouseMotionEvent, padding1));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "uint8 padding2", asOFFSET(graphics::MouseMotionEvent, padding2));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "uint8 padding3", asOFFSET(graphics::MouseMotionEvent, padding3));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "int32 x", asOFFSET(graphics::MouseMotionEvent, x));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "int32 y", asOFFSET(graphics::MouseMotionEvent, y));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "int32 xrel", asOFFSET(graphics::MouseMotionEvent, xrel));
	scriptingEngine_->registerObjectProperty("MouseMotionEvent", "int32 yrel", asOFFSET(graphics::MouseMotionEvent, yrel));
	scriptingEngine_->registerObjectType("MouseButtonEvent", sizeof(graphics::MouseButtonEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::MouseButtonEvent>());
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "uint32 type", asOFFSET(graphics::MouseButtonEvent, type));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "uint8 button", asOFFSET(graphics::MouseButtonEvent, button));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "uint8 state", asOFFSET(graphics::MouseButtonEvent, state));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "uint8 clicks", asOFFSET(graphics::MouseButtonEvent, clicks));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "uint8 padding3", asOFFSET(graphics::MouseButtonEvent, padding3));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "int32 x", asOFFSET(graphics::MouseButtonEvent, x));
	scriptingEngine_->registerObjectProperty("MouseButtonEvent", "int32 y", asOFFSET(graphics::MouseButtonEvent, y));
	scriptingEngine_->registerObjectType("MouseWheelEvent", sizeof(graphics::MouseWheelEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::MouseWheelEvent>());
	scriptingEngine_->registerObjectProperty("MouseWheelEvent", "uint32 type", asOFFSET(graphics::MouseWheelEvent, type));
	scriptingEngine_->registerObjectProperty("MouseWheelEvent", "int32 x", asOFFSET(graphics::MouseWheelEvent, x));
	scriptingEngine_->registerObjectProperty("MouseWheelEvent", "int32 y", asOFFSET(graphics::MouseWheelEvent, y));
	scriptingEngine_->registerObjectProperty("MouseWheelEvent", "uint32 direction", asOFFSET(graphics::MouseWheelEvent, direction));
	
	scriptingEngine_->registerObjectType("GraphicsColor", sizeof(graphics::Color), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::Color>());
	scriptingEngine_->registerObjectBehaviour("GraphicsColor", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<graphics::Color>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("GraphicsColor", asBEHAVE_CONSTRUCT, "void f(const GraphicsColor& in)", asFUNCTION(CopyConstructor<graphics::Color>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("GraphicsColor", asBEHAVE_CONSTRUCT, "void f(const uint32)", asFUNCTIONPR(InitConstructorGraphicsColor, (graphics::Color*, const uint32), void), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("GraphicsColor", asBEHAVE_CONSTRUCT, "void f(const uint8, const uint8, const uint8, const uint8)", asFUNCTIONPR(InitConstructorGraphicsColor, (graphics::Color*, const uint8, const uint8, const uint8, const uint8), void), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("GraphicsColor", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<graphics::Color>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("GraphicsColor", "void setColor(const uint32)", asMETHODPR(graphics::Color, setColor, (const uint32), void));
	scriptingEngine_->registerClassMethod("GraphicsColor", "void setColor(const uint8, const uint8, const uint8, const uint8)", asMETHODPR(graphics::Color, setColor, (const uint8, const uint8, const uint8, const uint8), void));
	scriptingEngine_->registerClassMethod("GraphicsColor", "uint32 color() const", asMETHOD(graphics::Color, color));

	scriptingEngine_->registerObjectType("Style", sizeof(graphics::gui::Style), asOBJ_VALUE | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<graphics::gui::Style>());
	scriptingEngine_->registerObjectBehaviour("Style", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<graphics::gui::Style>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Style", asBEHAVE_CONSTRUCT, "void f(const Style& in)", asFUNCTION(CopyConstructor<graphics::gui::Style>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Style", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<graphics::gui::Style>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectProperty("Style", "float alpha", asOFFSET(graphics::gui::Style, alpha));

	scriptingEngine_->registerObjectType("IMesh", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("ITexture", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("ISkeleton", 0, asOBJ_REF | asOBJ_NOCOUNT);
	
	registerSharedFutureBindings<graphics::VertexShaderHandle>(scriptingEngine_, "shared_futureVertexShaderHandle", "VertexShaderHandle");
	registerSharedFutureBindings<graphics::FragmentShaderHandle>(scriptingEngine_, "shared_futureFragmentShaderHandle", "FragmentShaderHandle");
	registerSharedFutureBindings<graphics::ShaderProgramHandle>(scriptingEngine_, "shared_futureShaderProgramHandle", "ShaderProgramHandle");
	
	// Register function declarations
	scriptingEngine_->registerFunctionDefinition("void ButtonClickCallback()");
	scriptingEngine_->registerFunctionDefinition("void MenuItemClickCallback()");
	
	// Gui
#define COMPONENT_CLASS_METHODS(name, class) \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setPosition(const uint32, const uint32)", \
		asFUNCTION(proxyFunctionSetPosition<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"ivec2 getPosition() const", \
		asFUNCTION(proxyFunctionGetPosition<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setDimensions(const uint32, const uint32)", \
		asFUNCTION(proxyFunctionSetDimensions<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"ivec2 getDimensions() const", \
		asFUNCTION(proxyFunctionGetDimensions<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setStyle(const Style& in)", \
		asFUNCTION(proxyFunctionSetStyle<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"const Style& getStyle() const", \
		asFUNCTION(proxyFunctionGetStyle<class>), \
		asCALL_CDECL_OBJLAST \
	); \

	scriptingEngine_->registerObjectType("IComponent", 0, asOBJ_REF | asOBJ_NOCOUNT);
	COMPONENT_CLASS_METHODS("IComponent", graphics::gui::IComponent)
	
	scriptingEngine_->registerObjectType("ILabel", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("ILabel", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::ILabel, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("ILabel", graphics::gui::ILabel)
	scriptingEngine_->registerClassMethod(
		"ILabel",
		"void setLabel(const string& in)",
		asMETHODPR(graphics::gui::ILabel, setLabel, (const std::string&), void)
	);
	scriptingEngine_->registerObjectType("IButton", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IButton", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IButton, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IButton", graphics::gui::IButton)
	scriptingEngine_->registerObjectMethod(
		"IButton",
		"void setCallback(ButtonClickCallback@ callback)",
		asMETHODPR(GameEngine, setCallback, (graphics::gui::IButton*, void*), void),
		asCALL_THISCALL_OBJFIRST,
		gameEngine_
	);
	scriptingEngine_->registerObjectType("IMenuItem", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IMenuItem", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IMenuItem, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IMenuItem", graphics::gui::IMenuItem)
	scriptingEngine_->registerObjectMethod(
		"IMenuItem",
		"void setCallback(MenuItemClickCallback@ callback)",
		asMETHODPR(GameEngine, setCallback, (graphics::gui::IMenuItem*, void*), void),
		asCALL_THISCALL_OBJFIRST,
		gameEngine_
	);
	scriptingEngine_->registerObjectType("IMenu", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IMenu", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IMenu, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IMenu", graphics::gui::IMenu)
	scriptingEngine_->registerClassMethod(
		"IMenu",
		"IMenu@ createMenu(const string& in)",
		asMETHODPR(graphics::gui::IMenu, createMenu, (const std::string&), graphics::gui::IMenu*)
	);
	scriptingEngine_->registerClassMethod(
		"IMenu",
		"void createSeparator()",
		asMETHODPR(graphics::gui::IMenu, createSeparator, (), void)
	);
	scriptingEngine_->registerClassMethod(
		"IMenu",
		"IMenuItem@ createMenuItem(const string& in)",
		asMETHODPR(graphics::gui::IMenu, createMenuItem, (const std::string&), graphics::gui::IMenuItem*)
	);
	scriptingEngine_->registerObjectType("IMenuBar", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IMenuBar", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IMenuBar, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IMenuBar", graphics::gui::IMenuBar)
	scriptingEngine_->registerClassMethod(
		"IMenuBar",
		"IMenu@ createMenu(const string& in)",
		asMETHODPR(graphics::gui::IMenuBar, createMenu, (const std::string&), graphics::gui::IMenu*)
	);
	scriptingEngine_->registerObjectType("IRectangle", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IRectangle", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IRectangle, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IRectangle", graphics::gui::IRectangle)
	scriptingEngine_->registerClassMethod(
		"IRectangle",
		"void setPoints(const vec2& in, const vec2& in)",
		asMETHOD(graphics::gui::IRectangle, setPoints)
	);
	scriptingEngine_->registerObjectType("IWindow", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod("IWindow", "IComponent@ opImplCast()", asFUNCTION((refCast<graphics::gui::IWindow, graphics::gui::IComponent>)), asCALL_CDECL_OBJLAST);
	COMPONENT_CLASS_METHODS("IWindow", graphics::gui::IWindow)
	scriptingEngine_->registerClassMethod("IWindow", "void setBackgroundAlpha(const float)", asMETHOD(graphics::gui::IWindow, setBackgroundAlpha));
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"ILabel@ createLabel(const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IWindow, createLabel, (const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::ILabel*)
	);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"IMenuBar@ createMenuBar()",
		asMETHODPR(graphics::gui::IWindow, createMenuBar, (), graphics::gui::IMenuBar*)
	);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"IButton@ createButton(const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IWindow, createButton, (const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::IButton*)
	);
	scriptingEngine_->registerClassMethod(
			"IWindow",
			"IRectangle@ createRectangle(const vec2& in, const vec2& in, const GraphicsColor& in)",
			asMETHOD(graphics::gui::IWindow, createRectangle)
		);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"void destroy(const IButton@)",
		asMETHODPR(graphics::gui::IWindow, destroy, (const graphics::gui::IButton*), void)
	);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"void destroy(const ILabel@)",
		asMETHODPR(graphics::gui::IWindow, destroy, (const graphics::gui::ILabel*), void)
	);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"void destroy(const IRectangle@)",
		asMETHODPR(graphics::gui::IWindow, destroy, (const graphics::gui::IRectangle*), void)
	);
	scriptingEngine_->registerObjectType("IGui", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IGui",
		"IWindow@ createWindow(const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IGui, createWindow, (const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::IWindow*)
	);
	scriptingEngine_->registerClassMethod(
		"IGui",
		"IWindow@ createWindow(const uint32, const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IGui, createWindow, (const uint32, const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::IWindow*)
	);
	
	// IGraphicsEngine
	scriptingEngine_->registerObjectType("IGraphicsEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IGraphicsEngine graphics", graphicsEngine_);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"uvec2 getViewport() const",
		asMETHODPR(graphics::IGraphicsEngine, getViewport, () const, glm::uvec2)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"mat4 getModelMatrix() const",
		asMETHODPR(graphics::IGraphicsEngine, getModelMatrix, () const, glm::mat4)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"mat4 getViewMatrix() const",
		asMETHODPR(graphics::IGraphicsEngine, getViewMatrix, () const, glm::mat4)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"mat4 getProjectionMatrix() const",
		asMETHODPR(graphics::IGraphicsEngine, getProjectionMatrix, () const, glm::mat4)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"CameraHandle createCamera(const vec3& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, createCamera, (const glm::vec3&, const glm::vec3&), graphics::CameraHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"PointLightHandle createPointLight(const RenderSceneHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, createPointLight, (const graphics::RenderSceneHandle&, const glm::vec3&), graphics::PointLightHandle)
	);
//	scriptingEngine_->registerClassMethod(
//		"IGraphicsEngine",
//		"MeshHandle createStaticMesh(const IMesh@)",
//		asMETHODPR(graphics::IGraphicsEngine, createStaticMesh, (const graphics::IMesh*), graphics::MeshHandle)
//	);
//	scriptingEngine_->registerClassMethod(
//		"IGraphicsEngine",
//		"TextureHandle createTexture2d(ITexture@)",
//		asMETHODPR(graphics::IGraphicsEngine, createTexture2d, (const graphics::ITexture*), graphics::TextureHandle)
//	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"RenderableHandle createRenderable(const RenderSceneHandle& in, const MeshHandle& in, const TextureHandle& in, const vec3& in, const quat& in, const vec3& in = vec3(1.0f), const ShaderProgramHandle& in = ShaderProgramHandle())",
		asMETHODPR(graphics::IGraphicsEngine, createRenderable, (const graphics::RenderSceneHandle&, const graphics::MeshHandle&, const graphics::TextureHandle&, const glm::vec3&, const glm::quat&, const glm::vec3&, const graphics::ShaderProgramHandle&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void rotate(const CameraHandle& in, const quat& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(graphics::IGraphicsEngine, rotate, (const graphics::CameraHandle&, const glm::quat&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void rotate(const CameraHandle& in, const float, const vec3& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(graphics::IGraphicsEngine, rotate, (const graphics::CameraHandle&, const float32, const glm::vec3&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void rotation(const CameraHandle& in, const quat& in)",
		asMETHODPR(graphics::IGraphicsEngine, rotation, (const graphics::CameraHandle&, const glm::quat&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"quat rotation(const CameraHandle& in) const",
		asMETHODPR(graphics::IGraphicsEngine, rotation, (const graphics::CameraHandle&) const, glm::quat)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void translate(const CameraHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, translate, (const graphics::CameraHandle&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void translate(const RenderSceneHandle& in, const PointLightHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, translate, (const graphics::RenderSceneHandle&, const graphics::PointLightHandle&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void position(const CameraHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, position, (const graphics::CameraHandle&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void position(const RenderSceneHandle& in, const PointLightHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, position, (const graphics::RenderSceneHandle&, const graphics::PointLightHandle&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void position(const CameraHandle& in, const float, const float, const float)",
		asMETHODPR(graphics::IGraphicsEngine, position, (const graphics::CameraHandle&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"vec3 position(const CameraHandle& in) const",
		asMETHODPR(graphics::IGraphicsEngine, position, (const graphics::CameraHandle&) const, glm::vec3)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void lookAt(const CameraHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, lookAt, (const graphics::CameraHandle&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void setMouseRelativeMode(const bool)",
		asMETHODPR(graphics::IGraphicsEngine, setMouseRelativeMode, (const bool), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void setWindowGrab(const bool)",
		asMETHODPR(graphics::IGraphicsEngine, setWindowGrab, (const bool), void)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"void setCursorVisible(const bool)",
		asMETHODPR(graphics::IGraphicsEngine, setCursorVisible, (const bool), void)
	);
}
	
};
