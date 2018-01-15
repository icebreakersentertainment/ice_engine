#ifndef BINDINGDELEGATEUTILITIES_H_
#define BINDINGDELEGATEUTILITIES_H_

#include <string>
#include <chrono>
#include <future>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "scripting/IScriptingEngine.hpp"

namespace ice_engine
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
	
	scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(HandleBase::template InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
	scriptingEngine->registerClassMethod(name.c_str(), "uint64 id() const", asMETHODPR(T, id, () const, uint64));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool )); 
	scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

}

#endif /* BINDINGDELEGATEUTILITIES_H_ */

