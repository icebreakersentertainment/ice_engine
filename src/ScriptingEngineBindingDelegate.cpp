#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "IScene.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "ScriptingEngineBindingDelegate.hpp"

#include "scripting/IScriptingEngine.hpp"

//#include "scripting/angel_script/scriptvector/scriptvector.hpp"

#include "GameEngine.hpp"

namespace hercules
{

template<typename T, typename V>
class VectorRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	static void InitConstructor(int size, T* memory) { new(memory) T(size); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
	
	static T& assignmentOperator(const T& other, T* v) { (*v) = other; return *v; }
	
	static void assign(int count, const V& value, T* v) { v->assign(count, value); }
	static int size(T* v) { return v->size(); }
	static void resize(int size, T* v) { v->resize(size); }
	static V& at(int i, T* v) { return v->at(i); }
	static V& index(int i, T* v) { return (*v)[i]; }
	static V& front(T* v) { return v->front(); }
	static V& back(T* v) { return v->back(); }
	static void push_back(const V& value, T* v) { v->push_back(value); }
	static void pop_back(T* v) { v->pop_back(); }
	static void erase(int i, T* v) { v->erase(v->begin() + i); }
	static void insert(int i, const V& value, T* v) { v->insert(v->begin() + i, value); }
	static void clear(T* v) { v->clear(); }
	static bool empty(T* v) { return v->empty(); }
	static int max_size(T* v) { return v->max_size(); }
	static void reserve(int size, T* v) { v->reserve(size); }
	static int capacity(T* v) { return v->capacity(); }
};


/**
 * Register our vector bindings.
 */
template<typename V>
void registerVectorBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	
	typedef VectorRegisterHelper<std::vector<V>, V> VectorBase;
	
	scriptingEngine->registerObjectType(name.c_str(), sizeof(std::vector<V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::vector<V>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VectorBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(VectorBase::CopyConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(VectorBase::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VectorBase::DefaultDestructor), asCALL_CDECL_OBJLAST);
	
	auto assignFunctionString = std::string("void assign(int, const ") + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignFunctionString.c_str(), asFUNCTION(VectorBase::assign), asCALL_CDECL_OBJLAST);
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(VectorBase::assignmentOperator), asCALL_CDECL_OBJLAST);
	auto pushBackFunctionString = "void push_back(" + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), pushBackFunctionString.c_str(), asFUNCTION(VectorBase::push_back), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void erase(int)", asFUNCTION(VectorBase::erase), asCALL_CDECL_OBJLAST);
	auto insertFunctionString = "void insert(int, const " + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), insertFunctionString.c_str(), asFUNCTION(VectorBase::insert), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void pop_back()", asFUNCTION(VectorBase::pop_back), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "int size() const", asFUNCTION(VectorBase::size), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void resize(int)", asFUNCTION(VectorBase::resize), asCALL_CDECL_OBJLAST);
	auto atFunctionString = type + "& at(int)";
	scriptingEngine->registerObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST);
	auto atConstFunctionString = std::string("const ") + type + "& at(int) const";
	scriptingEngine->registerObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST);
	auto indexFunctionString = type + "& opIndex(int)";
	scriptingEngine->registerObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(VectorBase::index), asCALL_CDECL_OBJLAST);
	auto indexConstFunctionString = std::string("const ") + type + "& opIndex(int) const";
	scriptingEngine->registerObjectMethod(name.c_str(), indexConstFunctionString.c_str(), asFUNCTION(VectorBase::index), asCALL_CDECL_OBJLAST);
	auto frontFunctionString = type + "& front()";
	scriptingEngine->registerObjectMethod(name.c_str(), frontFunctionString.c_str(), asFUNCTION(VectorBase::front), asCALL_CDECL_OBJLAST);
	auto frontConstFunctionString = std::string("const ") + type + "& front() const";
	scriptingEngine->registerObjectMethod(name.c_str(), frontConstFunctionString.c_str(), asFUNCTION(VectorBase::front), asCALL_CDECL_OBJLAST);
	auto backFunctionString = type + "& back()";
	scriptingEngine->registerObjectMethod(name.c_str(), backFunctionString.c_str(), asFUNCTION(VectorBase::back), asCALL_CDECL_OBJLAST);
	auto backConstFunctionString = std::string("const ") + type + "& back() const";
	scriptingEngine->registerObjectMethod(name.c_str(), backConstFunctionString.c_str(), asFUNCTION(VectorBase::back), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void clear()", asFUNCTION(VectorBase::clear), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "bool empty() const", asFUNCTION(VectorBase::empty), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "int max_size() const", asFUNCTION(VectorBase::max_size), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void reserve(int)", asFUNCTION(VectorBase::reserve), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "int capacity() const", asFUNCTION(VectorBase::capacity), asCALL_CDECL_OBJLAST);
}

template<typename T, typename V>
class FutureRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	//static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	//static void InitConstructor(int size, T* memory) { new(memory) T(size); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
	
	static T& assignmentOperator(const T& other, T* v) { (*v) = std::move(const_cast<T&>(other)); return *v; }
	
	static V get(T* v) { return v->get(); }
	static bool valid(T* v) { return v->valid(); }
	static void wait(T* v) { v->wait(); }
	static std::future_status wait_for(const std::chrono::seconds& s, T* v) { return v->wait_for(s); }
	//static std::future_status wait_until(const std::chrono::system_clock::time_point& timePoint, T* v) { return v->wait_until(timePoint); }
};

/**
 * Register our future bindings.
 */
template<typename V>
void registerFutureBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	
	typedef FutureRegisterHelper<std::future<V>, V> FutureBase;
	
	scriptingEngine->registerObjectType(name.c_str(), sizeof(std::future<V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::future<V>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(FutureBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
	//auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	//scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(FutureBase::CopyConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(FutureBase::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(FutureBase::DefaultDestructor), asCALL_CDECL_OBJLAST);
	
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(FutureBase::assignmentOperator), asCALL_CDECL_OBJLAST);
	auto getFunctionString = type + " get()";
	scriptingEngine->registerObjectMethod(name.c_str(), getFunctionString.c_str(), asFUNCTION(FutureBase::get), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "bool valid() const", asFUNCTION(FutureBase::valid), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void wait() const", asFUNCTION(FutureBase::wait), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_for(const chrono::seconds& in) const", asFUNCTION(FutureBase::wait_for), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_until(chrono::system_clock::time_point) const", asFUNCTION(FutureBase::wait_for), asCALL_CDECL_OBJLAST);
}

template<typename T, typename V>
class SharedFutureRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	//static void InitConstructor(int size, T* memory) { new(memory) T(size); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
	
	static T& assignmentOperator(const T& other, T* v) { (*v) = other; return *v; }
	
	static V get(T* v) { return v->get(); }
	static bool valid(T* v) { return v->valid(); }
	static void wait(T* v) { v->wait(); }
	static std::future_status wait_for(const std::chrono::seconds& s, T* v) { return v->wait_for(s); }
	//static std::future_status wait_until(const std::chrono::system_clock::time_point& timePoint, T* v) { return v->wait_until(timePoint); }
};

/**
 * Register our shared_future bindings.
 */
template<typename V>
void registerSharedFutureBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	
	typedef SharedFutureRegisterHelper<std::shared_future<V>, V> SharedFutureBase;
	
	scriptingEngine->registerObjectType(name.c_str(), sizeof(std::shared_future<V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::shared_future<V>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(SharedFutureBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(SharedFutureBase::CopyConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(SharedFutureBase::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(SharedFutureBase::DefaultDestructor), asCALL_CDECL_OBJLAST);
	
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(SharedFutureBase::assignmentOperator), asCALL_CDECL_OBJLAST);
	auto getFunctionString = type + " get()";
	scriptingEngine->registerObjectMethod(name.c_str(), getFunctionString.c_str(), asFUNCTION(SharedFutureBase::get), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "bool valid() const", asFUNCTION(SharedFutureBase::valid), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void wait() const", asFUNCTION(SharedFutureBase::wait), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_for(const chrono::seconds& in) const", asFUNCTION(SharedFutureBase::wait_for), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_until(chrono::system_clock::time_point) const", asFUNCTION(SharedFutureBase::wait_for), asCALL_CDECL_OBJLAST);
}

template<typename T>
class HandleRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	
	template<typename... Args>
	static void InitConstructor(T* memory, Args&&... args) { new(memory) T(std::forward<Args>(args)...); }
	
	static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
};

/**
 * Register our Handle bindings.
 */
template<typename T>
void registerHandleBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
	typedef HandleRegisterHelper<T> HandleBase;
	
	scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asGetTypeTraits<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(HandleBase::template InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
	scriptingEngine->registerClassMethod(name.c_str(), "uint64 id() const", asMETHODPR(T, id, () const, uint64));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool )); 
	scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

namespace idebugrenderer
{
void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, IDebugRenderer* debugRenderer)
{
	debugRenderer->pushLine(from, to, color);
}
}

namespace raybinding
{
void InitConstructor(const glm::vec3& from, const glm::vec3& to, void* memory) { new(memory) ray::Ray(from, to); }
}

ScriptingEngineBindingDelegate::ScriptingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	graphicsEngine_(graphicsEngine),
	physicsEngine_(physicsEngine),
	pathfindingEngine_(pathfindingEngine)
{
}

ScriptingEngineBindingDelegate::~ScriptingEngineBindingDelegate()
{
}

void ScriptingEngineBindingDelegate::bind()
{
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
	scriptingEngine_->registerEnumValue("EventType", "KEYDOWN", graphics::KEYDOWN);
	scriptingEngine_->registerEnumValue("EventType", "KEYUP", graphics::KEYUP);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEMOTION", graphics::MOUSEMOTION);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEBUTTONDOWN", graphics::MOUSEBUTTONDOWN);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEBUTTONUP", graphics::MOUSEBUTTONUP);
	scriptingEngine_->registerEnumValue("EventType", "MOUSEWHEEL", graphics::MOUSEWHEEL);
		
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
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_TITLE_BAR", graphics::gui::HERCULES_TITLE_BAR);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_SHOW_BORDERS", graphics::gui::HERCULES_SHOW_BORDERS);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_MOVABLE", graphics::gui::HERCULES_MOVABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_NO_SCROLLBAR", graphics::gui::HERCULES_NO_SCROLLBAR);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_CLOSABLE", graphics::gui::HERCULES_CLOSABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_MINIMIZABLE", graphics::gui::HERCULES_MINIMIZABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_RESIZABLE", graphics::gui::HERCULES_RESIZABLE);
	scriptingEngine_->registerEnumValue("WindowFlags", "HERCULES_NO_INPUT", graphics::gui::HERCULES_NO_INPUT);
	
	// Types available in the scripting engine
	registerHandleBindings<entities::Entity>(scriptingEngine_, "Entity");
	
	registerHandleBindings<ModelHandle>(scriptingEngine_, "ModelHandle");
	
	registerHandleBindings<graphics::CameraHandle>(scriptingEngine_, "CameraHandle");
	registerHandleBindings<graphics::RenderSceneHandle>(scriptingEngine_, "RenderSceneHandle");
	registerHandleBindings<graphics::MeshHandle>(scriptingEngine_, "MeshHandle");
	registerHandleBindings<graphics::TextureHandle>(scriptingEngine_, "TextureHandle");
	registerHandleBindings<graphics::RenderableHandle>(scriptingEngine_, "RenderableHandle");
	registerHandleBindings<graphics::ShaderHandle>(scriptingEngine_, "ShaderHandle");
	registerHandleBindings<graphics::ShaderProgramHandle>(scriptingEngine_, "ShaderProgramHandle");
	
	registerHandleBindings<physics::PhysicsSceneHandle>(scriptingEngine_, "PhysicsSceneHandle");
	registerHandleBindings<physics::CollisionShapeHandle>(scriptingEngine_, "CollisionShapeHandle");
	
	scriptingEngine_->registerObjectType("RigidBodyObjectHandle", sizeof(physics::RigidBodyObjectHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<physics::RigidBodyObjectHandle>());
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint64 id() const", asMETHODPR(physics::RigidBodyObjectHandle, id, () const, uint64));
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint32 index() const", asMETHODPR(physics::RigidBodyObjectHandle, index, () const, uint32));
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint32 version() const", asMETHODPR(physics::RigidBodyObjectHandle, version, () const, uint32));
	scriptingEngine_->registerObjectType("GhostObjectHandle", sizeof(physics::GhostObjectHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<physics::GhostObjectHandle>());
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint64 id() const", asMETHODPR(physics::GhostObjectHandle, id, () const, uint64));
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint32 index() const", asMETHODPR(physics::GhostObjectHandle, index, () const, uint32));
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint32 version() const", asMETHODPR(physics::GhostObjectHandle, version, () const, uint32));
	
	scriptingEngine_->registerObjectType("GraphicsComponent", sizeof(entities::GraphicsComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::GraphicsComponent>());
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "vec3 scale", asOFFSET(entities::GraphicsComponent, scale));
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "RenderableHandle renderableHandle", asOFFSET(entities::GraphicsComponent, renderableHandle));
	scriptingEngine_->registerObjectType("RigidBodyObjectComponent", sizeof(entities::RigidBodyObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::RigidBodyObjectComponent>());
	scriptingEngine_->registerObjectProperty("RigidBodyObjectComponent", "RigidBodyObjectHandle rigidBodyObjectHandle", asOFFSET(entities::RigidBodyObjectComponent, rigidBodyObjectHandle));
	scriptingEngine_->registerObjectType("GhostObjectComponent", sizeof(entities::GhostObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::GhostObjectComponent>());
	scriptingEngine_->registerObjectProperty("GhostObjectComponent", "GhostObjectHandle ghostObjectHandle", asOFFSET(entities::GhostObjectComponent, ghostObjectHandle));
	scriptingEngine_->registerObjectType("PositionOrientationComponent", sizeof(entities::PositionOrientationComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::PositionOrientationComponent>());
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "vec3 position", asOFFSET(entities::PositionOrientationComponent, position));
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "quat orientation", asOFFSET(entities::PositionOrientationComponent, orientation));
	
	//scriptingEngine_->registerObjectType("KeySym", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("KeySym", sizeof(graphics::KeySym), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<graphics::KeySym>());
	scriptingEngine_->registerObjectProperty("KeySym", "ScanCode scancode", asOFFSET(graphics::KeySym, scancode));
	scriptingEngine_->registerObjectProperty("KeySym", "KeyCode sym", asOFFSET(graphics::KeySym, sym));
	scriptingEngine_->registerObjectProperty("KeySym", "KeyMod mod", asOFFSET(graphics::KeySym, mod));
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmKeySym::DefaultConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f(const KeySym &in)", asFUNCTION(glmKeySym::CopyConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine_->registerObjectBehaviour("KeySym", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(glmKeySym::InitConstructor), asCALL_CDECL_OBJLAST);
	
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
	
	scriptingEngine_->registerObjectType("Ray", sizeof(ray::Ray), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<ray::Ray>());
	scriptingEngine_->registerObjectBehaviour("Ray", asBEHAVE_CONSTRUCT, "void f(const vec3& in, const vec3& in)", asFUNCTION(raybinding::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectProperty("Ray", "vec3 from", asOFFSET(ray::Ray, from));
	scriptingEngine_->registerObjectProperty("Ray", "vec3 to", asOFFSET(ray::Ray, to));
	
	scriptingEngine_->registerObjectType("Raycast", sizeof(Raycast), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<Raycast>());
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setRay(const Ray& in)",
		asMETHODPR(Raycast, setRay, (const ray::Ray&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const Ray& ray() const",
		asMETHODPR(Raycast, ray, () const, const ray::Ray&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setHitPointWorld(const vec3& in)",
		asMETHODPR(Raycast, setHitPointWorld, (const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const vec3& hitPointWorld() const",
		asMETHODPR(Raycast, hitPointWorld, () const, const glm::vec3&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setHitNormalWorld(const vec3& in)",
		asMETHODPR(Raycast, setHitNormalWorld, (const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const vec3& hitNormalWorld() const",
		asMETHODPR(Raycast, hitNormalWorld, () const, const glm::vec3&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setEntity(const Entity& in)",
		asMETHODPR(Raycast, setEntity, (const entities::Entity&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const Entity& entity() const",
		asMETHODPR(Raycast, entity, () const, const entities::Entity&)
	);
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 from", asOFFSET(ray::Ray, from));
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 to", asOFFSET(ray::Ray, to));
	
	// Register Model/Mesh/etc
	//registerVectorBindings<glm::vec3>(engine_, "vectorVec3", "vec3");
	//scriptingEngine_->registerObjectType("Mesh", 0, asOBJ_REF | asOBJ_NOCOUNT);
	//scriptingEngine_->registerObjectProperty("Mesh", "vectorMVec3 vertices", asOFFSET(graphics::model::Mesh, vertices));
	//scriptingEngine_->registerObjectType("Model", 0, asOBJ_REF | asOBJ_NOCOUNT);
	//scriptingEngine_->registerObjectType("Model", sizeof(graphics::model::Model), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<graphics::model::Model>());
	scriptingEngine_->registerObjectType("Model", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT);
	//scriptingEngine_->registerObjectProperty("Model", "vectorMesh meshes", asOFFSET(graphics::model::Model, meshes));
	
	// ILogger bindings
	scriptingEngine_->registerObjectType("ILogger", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("ILogger logger", logger_);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void info(const string& in)",
		asMETHODPR(logger::ILogger, info, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void debug(const string& in)",
		asMETHODPR(logger::ILogger, debug, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void warn(const string& in)",
		asMETHODPR(logger::ILogger, warn, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void error(const string& in)",
		asMETHODPR(logger::ILogger, error, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void fatal(const string& in)",
		asMETHODPR(logger::ILogger, fatal, (const std::string&), void)
	);
	
	// IThreadPool bindings
	scriptingEngine_->registerObjectType("IThreadPool", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getActiveWorkerCount()",
		asMETHODPR(IThreadPool, getActiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getInactiveWorkerCount()",
		asMETHODPR(IThreadPool, getInactiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueSize()",
		asMETHODPR(IThreadPool, getWorkQueueSize, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueCount()",
		asMETHODPR(IThreadPool, getWorkQueueCount, () const, uint32)
	);
	
	// IOpenGlLoader bindings
	scriptingEngine_->registerObjectType("IOpenGlLoader", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IOpenGlLoader",
		"int getWorkQueueCount()",
		asMETHODPR(IOpenGlLoader, getWorkQueueCount, () const, uint32)
	);
	
	// Future bindings
	scriptingEngine_->registerEnum("future_status");
	scriptingEngine_->registerEnumValue("future_status", "deferred", static_cast<int64>(std::future_status::deferred));
	scriptingEngine_->registerEnumValue("future_status", "ready", static_cast<int64>(std::future_status::ready));
	scriptingEngine_->registerEnumValue("future_status", "timeout", static_cast<int64>(std::future_status::timeout));
	
	registerSharedFutureBindings<graphics::model::Model*>(scriptingEngine_, "shared_futureModel", "Model@");
	registerSharedFutureBindings<image::Image*>(scriptingEngine_, "shared_futureImage", "Image@");
	registerSharedFutureBindings<ModelHandle>(scriptingEngine_, "shared_futureModelHandle", "ModelHandle");
	registerSharedFutureBindings<graphics::ShaderHandle>(scriptingEngine_, "shared_futureShaderHandle", "ShaderHandle");
	registerSharedFutureBindings<graphics::ShaderProgramHandle>(scriptingEngine_, "shared_futureShaderProgramHandle", "ShaderProgramHandle");
	
	scriptingEngine_->registerObjectType("EngineStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float fps", asOFFSET(EngineStatistics, fps));
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float renderTime", asOFFSET(EngineStatistics, renderTime));
	scriptingEngine_->registerObjectType("SceneStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float physicsTime", asOFFSET(SceneStatistics, physicsTime));
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float renderTime", asOFFSET(SceneStatistics, renderTime));
	
	// Register function declarations
	scriptingEngine_->registerFunctionDefinition("void ButtonClickCallback()");
	
	// IGame
	scriptingEngine_->registerInterface("IGame");
	scriptingEngine_->registerInterfaceMethod("IGame", "void initialize()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void destroy()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void tick(const float)");
	
	// ISceneThingy
	scriptingEngine_->registerInterface("ISceneThingy");
	scriptingEngine_->registerInterfaceMethod("ISceneThingy", "void preTick(const float)");
	scriptingEngine_->registerInterfaceMethod("ISceneThingy", "void postTick(const float)");
	
	// Gui
	scriptingEngine_->registerObjectType("ILabel", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"ILabel",
		"void setLabel(const string& in)",
		asMETHODPR(graphics::gui::ILabel, setLabel, (const std::string&), void)
	);
	scriptingEngine_->registerObjectType("IButton", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectMethod(
		"IButton",
		"void setCallback(ButtonClickCallback@ callback)",
		asMETHODPR(GameEngine, setCallback, (graphics::gui::IButton*, scripting::ScriptFunctionHandle), void),
		asCALL_THISCALL_OBJFIRST,
		gameEngine_
	);
	scriptingEngine_->registerObjectType("IWindow", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"ILabel@ createLabel(const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IWindow, createLabel, (const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::ILabel*)
	);
	scriptingEngine_->registerClassMethod(
		"IWindow",
		"IButton@ createButton(const uint32, const uint32, const uint32, const uint32, const string = string())",
		asMETHODPR(graphics::gui::IWindow, createButton, (const uint32, const uint32, const uint32, const uint32, const std::string), graphics::gui::IButton*)
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
	
	// Listeners
	scriptingEngine_->registerInterface("IKeyboardEventListener");
	scriptingEngine_->registerInterfaceMethod("IKeyboardEventListener", "bool processEvent(const KeyboardEvent& in)");
	scriptingEngine_->registerInterface("IMouseMotionEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseMotionEventListener", "bool processEvent(const MouseMotionEvent& in)");
	scriptingEngine_->registerInterface("IMouseButtonEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseButtonEventListener", "bool processEvent(const MouseButtonEvent& in)");
	scriptingEngine_->registerInterface("IMouseWheelEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseWheelEventListener", "bool processEvent(const MouseWheelEvent& in)");
	
	// IScene
	scriptingEngine_->registerObjectType("IScene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("IScene", "string getName() const", asMETHODPR(IScene, getName, () const, std::string));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"const SceneStatistics@ getSceneStatistics()",
		asMETHODPR(IScene, getSceneStatistics, () const, const SceneStatistics&)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(IScene, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticBoxShape(const vec3& in)",
		asMETHODPR(IScene, createStaticBoxShape, (const glm::vec3&), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyStaticShape(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, destroyStaticShape, (const physics::CollisionShapeHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyAllStaticShapes()",
		asMETHODPR(IScene, destroyAllStaticShapes, (), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createGhostObject, (const physics::CollisionShapeHandle&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in, const vec3& in, const quat& in)",
		asMETHODPR(IScene, createGhostObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RenderableHandle createRenderable(const ModelHandle& in, const ShaderProgramHandle& in, const string& in = string())",
		asMETHODPR(IScene, createRenderable, (const ModelHandle&, const graphics::ShaderProgramHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RenderableHandle createRenderable(const MeshHandle& in, const TextureHandle& in, const ShaderProgramHandle& in, const string& in = string())",
		asMETHODPR(IScene, createRenderable, (const graphics::MeshHandle&, const graphics::TextureHandle&, const graphics::ShaderProgramHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod("IScene", "Entity createEntity()", asMETHODPR(IScene, createEntity, (), entities::Entity));
	scriptingEngine_->registerClassMethod("IScene", "uint32 getNumEntities()", asMETHODPR(IScene, getNumEntities, () const, uint32));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"Raycast raycast(const Ray& in)",
		asMETHODPR(IScene, raycast, (const ray::Ray&), Raycast)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const GraphicsComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::GraphicsComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const RigidBodyObjectComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::RigidBodyObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const GhostObjectComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::GhostObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotate(const Entity& in, const quat& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(IScene, rotate, (const entities::Entity&, const glm::quat&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotate(const Entity& in, const float, const vec3& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(IScene, rotate, (const entities::Entity&, const float32, const glm::vec3&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotation(const Entity& in, const quat& in)",
		asMETHODPR(IScene, rotation, (const entities::Entity&, const glm::quat&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"quat rotation(const Entity& in) const",
		asMETHODPR(IScene, rotation, (const entities::Entity&) const, glm::quat)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"vec3 scale(const Entity& in) const",
		asMETHODPR(IScene, scale, (const entities::Entity&) const, glm::vec3)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void translate(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, translate, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, position, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, position, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"vec3 position(const Entity& in) const",
		asMETHODPR(IScene, position, (const entities::Entity&) const, glm::vec3)
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
		"MeshHandle createStaticMesh(const vectorVec3& in, const vectorUInt32& in, const vectorVec4& in, const vectorVec3& in, const vectorVec2& in)",
		asMETHODPR(graphics::IGraphicsEngine, createStaticMesh, (const std::vector<glm::vec3>&, const std::vector<uint32>&, const std::vector<glm::vec4>&, const std::vector<glm::vec3>&, const std::vector<glm::vec2>&), graphics::MeshHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"TextureHandle createTexture2d(Image@)",
		asMETHODPR(graphics::IGraphicsEngine, createTexture2d, (const image::Image&), graphics::TextureHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IGraphicsEngine",
		"RenderableHandle createRenderable(const RenderSceneHandle& in, const MeshHandle& in, const TextureHandle& in, const ShaderProgramHandle& in)",
		asMETHODPR(graphics::IGraphicsEngine, createRenderable, (const graphics::RenderSceneHandle&, const graphics::MeshHandle&, const graphics::TextureHandle&, const graphics::ShaderProgramHandle&), graphics::RenderableHandle)
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
		"void position(const CameraHandle& in, const vec3& in)",
		asMETHODPR(graphics::IGraphicsEngine, position, (const graphics::CameraHandle&, const glm::vec3&), void)
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
	
	// IDebugRenderer
	scriptingEngine_->registerObjectType("IDebugRenderer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IDebugRenderer debugRenderer", gameEngine_->getDebugRenderer());
	scriptingEngine_->registerObjectMethod(
		"IDebugRenderer",
		"void pushLine(const vec3& in, const vec3& in, const vec3& in)",
		asFUNCTION(idebugrenderer::pushLine),
		asCALL_CDECL_OBJLAST
	);
	
	// IPhysicsEngine
	scriptingEngine_->registerObjectType("IPhysicsEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IPhysicsEngine physics", physicsEngine_);
	scriptingEngine_->registerClassMethod(
		"IPhysicsEngine",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(physics::IPhysicsEngine, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IPhysicsEngine",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const PhysicsSceneHandle& in, const CollisionShapeHandle& in)",
		asMETHODPR(physics::IPhysicsEngine, createStaticRigidBodyObject, (const physics::PhysicsSceneHandle&, const physics::CollisionShapeHandle&, const physics::UserData&), physics::RigidBodyObjectHandle)
	);
	
	// IPathfindingEngine
	scriptingEngine_->registerObjectType("IPathfindingEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IPathfindingEngine pathfinding", pathfindingEngine_);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test(vectorVec3& in, vectorUInt32& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test, (std::vector<glm::vec3>&, std::vector<uint32>&), void )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test2(Entity, IScene@, const vec3& in, const quat& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test2, (entities::Entity, IScene*, const glm::vec3&, const glm::quat&), void )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test3(Entity, const vec3& in, const quat& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test3, (entities::Entity, const glm::vec3&, const glm::quat&), void )
	);
	
	// IGameEngine functions available in the scripting engine
	scriptingEngine_->registerGlobalFunction(
		"const EngineStatistics@ getEngineStatistics()",
		asMETHODPR(IGameEngine, getEngineStatistics, () const, const EngineStatistics&),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void setIGameInstance(IGame@)",
		asMETHODPR(GameEngine, setIGameInstance, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGraphicsEngine@ getGraphicsEngine()",
		asMETHODPR(IGameEngine, getGraphicsEngine, () const, graphics::IGraphicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IPhysicsEngine@ getPhysicsEngine()",
		asMETHODPR(IGameEngine, getPhysicsEngine, () const, physics::IPhysicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IDebugRenderer@ getDebugRenderer()",
		asMETHODPR(IGameEngine, getDebugRenderer, () const, IDebugRenderer*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getBackgroundThreadPool()",
		asMETHODPR(IGameEngine, getBackgroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getForegroundThreadPool()",
		asMETHODPR(IGameEngine, getForegroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IOpenGlLoader@ getOpenGlLoader()",
		asMETHODPR(IGameEngine, getOpenGlLoader, () const, IOpenGlLoader*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGui@ createGui(const string& in)",
		asMETHODPR(IGameEngine, createGui, (const std::string&), graphics::gui::IGui*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyGui(const IGui@)",
		asMETHODPR(IGameEngine, destroyGui, (const graphics::gui::IGui*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, addKeyboardEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, addMouseMotionEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, removeKeyboardEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, removeMouseMotionEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ importModel(const string& in, const string& in)",
		asMETHODPR(IGameEngine, importModel, (const std::string&, const std::string&), graphics::model::Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModel importModelAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, importModelAsync, (const std::string&, const std::string&), std::shared_future<graphics::model::Model*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ loadImage(const string& in, const string& in)",
		asMETHODPR(IGameEngine, loadImage, (const std::string&, const std::string&), image::Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureImage loadImageAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, loadImageAsync, (const std::string&, const std::string&), std::shared_future<image::Image*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ getModel(const string& in)",
		asMETHODPR(IGameEngine, getModel, (const std::string&) const, graphics::model::Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ getImage(const string& in)",
		asMETHODPR(IGameEngine, getImage, (const std::string&) const, image::Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadModel(const string& in)",
		asMETHODPR(IGameEngine, unloadModel, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadImage(const string& in)",
		asMETHODPR(IGameEngine, unloadImage, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ModelHandle loadStaticModel(Model@)",
		asMETHODPR(IGameEngine, loadStaticModel, (const graphics::model::Model*), ModelHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModelHandle loadStaticModelAsync(Model@)",
		asMETHODPR(IGameEngine, loadStaticModelAsync, (const graphics::model::Model*), std::shared_future<ModelHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle createVertexShader(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShader, (const std::string&, const std::string&), graphics::ShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderHandle createVertexShaderAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::ShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle createVertexShaderFromSource(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderFromSource, (const std::string&, const std::string&), graphics::ShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderHandle createVertexShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::ShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle createFragmentShader(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShader, (const std::string&, const std::string&), graphics::ShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderHandle createFragmentShaderAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::ShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle createFragmentShaderFromSource(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderFromSource, (const std::string&, const std::string&), graphics::ShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderHandle createFragmentShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::ShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle getShader(const string& in)",
		asMETHODPR(IGameEngine, getShader, (const std::string&) const, graphics::ShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const string& in)",
		asMETHODPR(IGameEngine, destroyShader, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderHandle destroyShader(const ShaderHandle& in)",
		asMETHODPR(IGameEngine, destroyShader, (const graphics::ShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle createShaderProgram(const string& in, const ShaderHandle& in, const ShaderHandle& in)",
		asMETHODPR(IGameEngine, createShaderProgram, (const std::string&, const graphics::ShaderHandle&, const graphics::ShaderHandle&), graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderProgramHandle createShaderProgramAsync(const string& in, const ShaderHandle& in, const ShaderHandle& in)",
		asMETHODPR(IGameEngine, createShaderProgramAsync, (const std::string&, const graphics::ShaderHandle&, const graphics::ShaderHandle&), std::shared_future<graphics::ShaderProgramHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle getShaderProgram(const string& in)",
		asMETHODPR(IGameEngine, getShaderProgram, (const std::string&) const, graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const string& in)",
		asMETHODPR(IGameEngine, destroyShaderProgram, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const ShaderProgramHandle& in)",
		asMETHODPR(IGameEngine, destroyShaderProgram, (const graphics::ShaderProgramHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ createScene(const string& in)",
		asMETHODPR(IGameEngine, createScene, (const std::string&), IScene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ getScene(const string& in)",
		asMETHODPR(IGameEngine, getScene, (const std::string&) const, IScene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(const string& in)",
		asMETHODPR(IGameEngine, destroyScene, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(IScene@)",
		asMETHODPR(IGameEngine, destroyScene, (IScene* scene), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	registerVectorBindings<IScene*>(scriptingEngine_, "vectorIScene", "IScene@");
	scriptingEngine_->registerGlobalFunction(
		"vectorIScene getAllScenes()",
		asMETHODPR(IGameEngine, getAllScenes, () const, std::vector<IScene*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
}
	
};
