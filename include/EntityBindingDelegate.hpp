#ifndef ENTITYBINDINGDELEGATE_H_
#define ENTITYBINDINGDELEGATE_H_

#include <string>
#include <vector>
#include <tuple>
#include <boost/algorithm/string/join.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ecs/Entity.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "BindingDelegateUtilities.hpp"

namespace ice_engine
{

namespace logger
{
class ILogger;
}

class GameEngine;

template <class C>
void registerComponentBehaviours(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
	scriptingEngine->registerObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<C>), asCALL_CDECL_OBJFIRST);
	/* scriptingEngine->registerObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST); */
	scriptingEngine->registerObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(CopyConstructor<C>), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<C>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine->registerClassMethod(name, name + "& opAssign(const " + name + "& in)", asMETHODPR(C, operator=, (const C&), C&));
	//scriptingEngine->registerClassMethod(name, "bool opEquals(const " + name + "& in) const", asMETHODPR(C, operator==, (const C&) const, bool));
}

template <class C>
void registerComponentMethods(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
	scriptingEngine->registerClassMethod(name, name + "& opAssign(const " + name + "& in)", asMETHODPR(C, operator=, (const C&), C&));
	//scriptingEngine->registerClassMethod(name, "bool opEquals(const " + name + "& in) const", asMETHODPR(C, operator==, (const C&) const, bool));
}

template <class C>
void registerComponentHandle(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
	scriptingEngine->registerObjectType("ComponentHandle" + name, sizeof(entityx::ComponentHandle<C>), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<entityx::ComponentHandle<C>>());
	scriptingEngine->registerObjectBehaviour("ComponentHandle" + name, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<entityx::ComponentHandle<C>>), asCALL_CDECL_OBJFIRST);
	/* scriptingEngine->registerObjectBehaviour("ComponentHandle" + name, asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST); */
	scriptingEngine->registerObjectBehaviour("ComponentHandle" + name, asBEHAVE_CONSTRUCT, "void f(const ComponentHandle" + name + "& in)", asFUNCTION(CopyConstructor<entityx::ComponentHandle<C>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour("ComponentHandle" + name, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<entityx::ComponentHandle<C>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod("ComponentHandle" + name, "ComponentHandle" + name + "& opAssign(const ComponentHandle" + name + "& in)", asMETHODPR(entityx::ComponentHandle<C>, operator=, (const entityx::ComponentHandle<C>&), entityx::ComponentHandle<C>&));
	scriptingEngine->registerClassMethod("ComponentHandle" + name, "bool opEquals(const ComponentHandle" + name + "& in) const", asMETHODPR(entityx::ComponentHandle<C>, operator==, (const entityx::ComponentHandle<C>&) const, bool));
	scriptingEngine->registerClassMethod("ComponentHandle" + name, "bool opImplConv() const", asMETHODPR(entityx::ComponentHandle<C>, operator bool, () const, bool ));
	scriptingEngine->registerClassMethod(
		"ComponentHandle" + name,
		name + "& get()",
		asMETHODPR(entityx::ComponentHandle<C>, get, (), C*)
	);
}


template <class C, typename ... Args>
void registerEntityComponentAssignMethod(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& params = std::string())
{
	scriptingEngine->registerClassMethod(
		"Entity",
		"ComponentHandle" + name + " assign" + name + "(" + params + ")",
		asMETHODPR(ecs::Entity, assign<C>, (Args&&... args), entityx::ComponentHandle<C>)
	);
}

template <class C, typename ... Args>
static entityx::ComponentHandle<C> assignNoForward(ecs::Entity& entity, Args... args)
{
	return entity.assign<C>(std::forward<Args>(args) ...);
}

#define COMMA ,
template <class C, typename ... Args>
void registerEntityComponentAssignMethodNoForward(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& params = std::string())
{
	scriptingEngine->registerObjectMethod(
		"Entity",
		"ComponentHandle" + name + " assign" + name + "(" + params + ")",
		asFUNCTION(assignNoForward<C COMMA Args...>),
		asCALL_CDECL_OBJFIRST
	);
}

template <class C, typename ... Args>
void registerEntityComponentMethods(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& params = std::string())
{
	registerEntityComponentAssignMethodNoForward<C>(scriptingEngine, name);

	if (sizeof...(Args) > 0)
	{
		registerEntityComponentAssignMethodNoForward<C, Args...>(scriptingEngine, name, params);
	}

	scriptingEngine->registerClassMethod(
		"Entity",
		"ComponentHandle" + name + " assignFromCopy" + name + "(const " + name + "& in)",
		asMETHOD(ecs::Entity, assignFromCopy<C>)
	);
	scriptingEngine->registerClassMethod(
		"Entity",
		"bool hasComponent" + name + "() const",
		asMETHODPR(ecs::Entity, hasComponent<C>, () const, bool)
	);
	scriptingEngine->registerClassMethod(
		"Entity",
		"ComponentHandle" + name + " component" + name + "()",
		asMETHODPR(ecs::Entity, component<C>, (), entityx::ComponentHandle<C>)
	);
}

template <class C, typename ... Args>
void registerComponent(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::vector<std::pair<std::string, size_t>>& memberVariables, const std::string& params)
{
	scriptingEngine->registerObjectType(name, sizeof(C), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<C>());

	for (const auto& memberVariable : memberVariables)
	{
		scriptingEngine->registerObjectProperty(name, std::get<0>(memberVariable), std::get<1>(memberVariable));
	}

	registerComponentBehaviours<C>(scriptingEngine, name);
	registerComponentMethods<C>(scriptingEngine, name);
	registerComponentHandle<C>(scriptingEngine, name);
	registerEntityComponentMethods<C, Args...>(scriptingEngine, name, params);
}

template <class C, typename ... Args>
void registerComponent(ice_engine::scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::vector<std::pair<std::string, size_t>>& memberVariables)
{
	scriptingEngine->registerObjectType(name, sizeof(C), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<C>());

	std::vector<std::string> parameters;

	for (const auto& memberVariable : memberVariables)
	{
		scriptingEngine->registerObjectProperty(name, std::get<0>(memberVariable), std::get<1>(memberVariable));

		parameters.push_back(std::get<0>(memberVariable));
	}

	const std::string parametersAsString= boost::algorithm::join(parameters, ",");

	registerComponentBehaviours<C>(scriptingEngine, name);
	registerComponentMethods<C>(scriptingEngine, name);
	registerComponentHandle<C>(scriptingEngine, name);
	registerEntityComponentMethods<C, Args...>(scriptingEngine, name, parametersAsString);
}

class EntityBindingDelegate
{
public:
	EntityBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine);
	~EntityBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
};

}

#endif /* ENTITYBINDINGDELEGATE_H_ */

