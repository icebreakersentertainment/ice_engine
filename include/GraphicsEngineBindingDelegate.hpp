#ifndef GRAPHICSENGINEBINDINGDELEGATE_H_
#define GRAPHICSENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

#define COMPONENT_CLASS_METHODS(name, class) \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setPosition(const uint32, const uint32)", \
		asFUNCTION(proxyFunctionSetPosition<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"ivec2 position() const", \
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
		"ivec2 dimensions() const", \
		asFUNCTION(proxyFunctionGetDimensions<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"bool visible() const", \
		asFUNCTION(proxyFunctionVisible<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setVisible(const bool)", \
		asFUNCTION(proxyFunctionSetVisible<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"bool disabled() const", \
		asFUNCTION(proxyFunctionDisabled<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void setDisabled(const bool)", \
		asFUNCTION(proxyFunctionSetDisabled<class>), \
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
		"const Style& style() const", \
		asFUNCTION(proxyFunctionGetStyle<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void addComponent(IComponent@)", \
		asFUNCTION(proxyFunctionAddComponent<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void removeComponent(IComponent@)", \
		asFUNCTION(proxyFunctionRemoveComponent<class>), \
		asCALL_CDECL_OBJLAST \
	); \
	scriptingEngine_->registerObjectMethod( \
		name, \
		"void removeAllComponents()", \
		asFUNCTION(proxyFunctionRemoveAllComponents<class>), \
		asCALL_CDECL_OBJLAST \
	); \

namespace scripting
{
class IScriptingEngine;
}
namespace graphics
{
class IGraphicsEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class GraphicsEngineBindingDelegate
{
public:
	GraphicsEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine);
	~GraphicsEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
};

}

#endif /* GRAPHICSENGINEBINDINGDELEGATE_H_ */

