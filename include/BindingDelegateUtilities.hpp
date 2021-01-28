#ifndef BINDINGDELEGATEUTILITIES_H_
#define BINDINGDELEGATEUTILITIES_H_

#include <string>
#include <sstream>
#include <unordered_map>
#include <list>
#include <chrono>
#include <future>
#include <type_traits>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <boost/variant/get.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "scripting/IScriptingEngine.hpp"

namespace ice_engine
{

#define COMMA ,

template<typename T>
std::function<std::string(void*)> scriptingEngineDebuggerToStringCallback()
{
    return [](void* value) {
        std::stringstream ss;
        ss << *static_cast<T*>(value);
        return ss.str();
    };
}

template<class T>
static void DefaultConstructor(T* memory) { new(memory) T(); }

template<class T, typename... Args>
static void InitConstructorNoForward(T* memory, Args... args) { new(memory) T(std::forward<Args>(args)...); }

template<class T>
static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }

template<class T>
static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

template<class T>
static T& defaultAssignmentOperator(const T& other, T* v) { (*v) = other; return *v; }

template<typename T, typename V>
class VectorRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	static void InitConstructor(uint64 size, T* memory) { new(memory) T(static_cast<size_t>(size)); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

	static T& assignmentOperator(const T& other, T* v) { (*v) = other; return *v; }

	static void assign(uint64 count, const V& value, T* v) { v->assign(static_cast<size_t>(count), value); }
    static uint64 size(T* v) { return static_cast<uint64>(v->size()); }
	static void resize(uint64 size, T* v) { v->resize(static_cast<size_t>(size)); }
	static V& at(uint64 i, T* v) { return v->at(static_cast<size_t>(i)); }
	static V& index(uint64 i, T* v) { return (*v)[static_cast<size_t>(i)]; }
	static V& front(T* v) { return v->front(); }
	static V& back(T* v) { return v->back(); }
	static void push_back(const V& value, T* v) { v->push_back(value); }
	static void pop_back(T* v) { v->pop_back(); }
	static void erase(uint64 i, T* v) { v->erase(v->begin() + static_cast<size_t>(i)); }
	static void insert(uint64 i, const V& value, T* v) { v->insert(v->begin() + static_cast<size_t>(i), value); }
	static void clear(T* v) { v->clear(); }
	static bool empty(T* v) { return v->empty(); }
	static uint64 max_size(T* v) { return static_cast<uint64>(v->max_size()); }
	static void reserve(uint64 size, T* v) { v->reserve(static_cast<size_t>(size)); }
	static uint64 capacity(T* v) { return static_cast<uint64>(v->capacity()); }
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
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(uint64)", asFUNCTION(VectorBase::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VectorBase::DefaultDestructor), asCALL_CDECL_OBJLAST);

	auto assignFunctionString = std::string("void assign(uint64, const ") + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignFunctionString.c_str(), asFUNCTION(VectorBase::assign), asCALL_CDECL_OBJLAST);
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(VectorBase::assignmentOperator), asCALL_CDECL_OBJLAST);
	auto pushBackFunctionString = "void push_back(" + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), pushBackFunctionString.c_str(), asFUNCTION(VectorBase::push_back), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void erase(uint64)", asFUNCTION(VectorBase::erase), asCALL_CDECL_OBJLAST);
	auto insertFunctionString = "void insert(uint64, const " + type + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), insertFunctionString.c_str(), asFUNCTION(VectorBase::insert), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void pop_back()", asFUNCTION(VectorBase::pop_back), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "uint64 size() const", asFUNCTION(VectorBase::size), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void resize(uint64)", asFUNCTION(VectorBase::resize), asCALL_CDECL_OBJLAST);
	auto atFunctionString = type + "& at(uint64)";
	scriptingEngine->registerObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST);
	auto atConstFunctionString = std::string("const ") + type + "& at(uint64) const";
	scriptingEngine->registerObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST);
	auto indexFunctionString = type + "& opIndex(uint64)";
	scriptingEngine->registerObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(VectorBase::index), asCALL_CDECL_OBJLAST);
	auto indexConstFunctionString = std::string("const ") + type + "& opIndex(uint64) const";
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
	scriptingEngine->registerObjectMethod(name.c_str(), "uint64 max_size() const", asFUNCTION(VectorBase::max_size), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void reserve(uint64)", asFUNCTION(VectorBase::reserve), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "uint64 capacity() const", asFUNCTION(VectorBase::capacity), asCALL_CDECL_OBJLAST);
}

template<typename T, typename K, typename V>
class UnorderedMapRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

	static T& assignmentOperator(const T& other, T* m) { (*m) = other; return *m; }

	static uint64 size(T* m) { return static_cast<uint64>(m->size()); }
	static V& at(const K& k, T* m) { return m->at(k); }
	static V& index(const K& k, T* m) { return (*m)[k]; }
	static void clear(T* m) { m->clear(); }
	static bool empty(T* m) { return m->empty(); }
	static uint64 max_size(T* m) { return static_cast<uint64>(m->max_size()); }
};

/**
 * Register our map bindings.
 */
template<typename K, typename V>
void registerUnorderedMapBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& keyType, const std::string& valueType, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{

	typedef UnorderedMapRegisterHelper<std::unordered_map<K, V>, K, V> UnorderedMapBase;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(std::unordered_map<K, V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::unordered_map<K, V>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(UnorderedMapBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(UnorderedMapBase::CopyConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(UnorderedMapBase::DefaultDestructor), asCALL_CDECL_OBJLAST);

	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(UnorderedMapBase::assignmentOperator), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "uint64 size() const", asFUNCTION(UnorderedMapBase::size), asCALL_CDECL_OBJLAST);
	auto atFunctionString = valueType + "& at(" + keyType + ")";
	scriptingEngine->registerObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(UnorderedMapBase::at), asCALL_CDECL_OBJLAST);
	auto atConstFunctionString = std::string("const ") + valueType + "& at(" + keyType + ") const";
	scriptingEngine->registerObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(UnorderedMapBase::at), asCALL_CDECL_OBJLAST);
	auto indexFunctionString = valueType + "& opIndex(" + keyType + ")";
	scriptingEngine->registerObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(UnorderedMapBase::index), asCALL_CDECL_OBJLAST);
	auto indexConstFunctionString = std::string("const ") + valueType + "& opIndex(" + keyType + ") const";
	scriptingEngine->registerObjectMethod(name.c_str(), indexConstFunctionString.c_str(), asFUNCTION(UnorderedMapBase::index), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void clear()", asFUNCTION(UnorderedMapBase::clear), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "bool empty() const", asFUNCTION(UnorderedMapBase::empty), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "uint64 max_size() const", asFUNCTION(UnorderedMapBase::max_size), asCALL_CDECL_OBJLAST);
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

    template <typename V1 = V>
    static
    typename std::enable_if<std::is_void<V1>::value, void>::type
    get(const T* v) { v->get(); }

    template <typename V1 = V>
	static const
    typename std::enable_if<std::is_pointer<V1>::value, V1>::type
    get(const T* v) { return v->get(); }

    template <typename V1 = V>
	static const
    typename std::enable_if<!std::is_void<V1>::value && !std::is_pointer<V1>::value, V1>::type&
    get(const T* v) { return v->get(); }

	static bool valid(T* v) { return v->valid(); }
	static void wait(T* v) { v->wait(); }
//	static std::future_status wait_for(const std::chrono::seconds& s, T* v) { return v->wait_for(s); }

    template <typename Duration>
	static std::future_status wait_for(const Duration& d, T* v) { return v->wait_for(d); }
	//static std::future_status wait_until(const std::chrono::system_clock::time_point& timePoint, T* v) { return v->wait_until(timePoint); }
};

/**
 * Register our shared_future bindings.
 */
template<typename V>
void registerSharedFutureBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{

	using SharedFutureType = SharedFutureRegisterHelper<std::shared_future<V>, V>;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(std::shared_future<V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::shared_future<V>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(SharedFutureType::DefaultConstructor), asCALL_CDECL_OBJLAST);
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(SharedFutureType::CopyConstructor), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(SharedFutureType::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(SharedFutureType::DefaultDestructor), asCALL_CDECL_OBJLAST);

	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(SharedFutureType::assignmentOperator), asCALL_CDECL_OBJLAST);

	if (type == "void")
    {
        auto getFunctionString = type + " get() const";
        scriptingEngine->registerObjectMethod(name.c_str(), getFunctionString.c_str(), asFUNCTION(SharedFutureType:: template get<>), asCALL_CDECL_OBJLAST);
    }
	else if (type.find("@") > 0)
    {
        auto getFunctionString = type + " get() const";
        scriptingEngine->registerObjectMethod(name.c_str(), getFunctionString.c_str(), asFUNCTION(SharedFutureType:: template get<>), asCALL_CDECL_OBJLAST);
    }
	else
    {
        auto getFunctionString = "const " + type + "& get() const";
        scriptingEngine->registerObjectMethod(name.c_str(), getFunctionString.c_str(), asFUNCTION(SharedFutureType:: template get<>), asCALL_CDECL_OBJLAST);
    }

	scriptingEngine->registerObjectMethod(name.c_str(), "bool valid() const", asFUNCTION(SharedFutureType::valid), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "void wait() const", asFUNCTION(SharedFutureType::wait), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_for(const chrono::durationFloat& in) const", asFUNCTION(SharedFutureType::wait_for<std::chrono::duration<float32>>), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_for(const chrono::seconds& in) const", asFUNCTION(SharedFutureType::wait_for<std::chrono::seconds>), asCALL_CDECL_OBJLAST);
	//scriptingEngine->registerObjectMethod(name.c_str(), "future_status wait_until(chrono::system_clock::time_point) const", asFUNCTION(SharedFutureType::wait_for), asCALL_CDECL_OBJLAST);
}

template<typename T, typename ... V>
class VariantRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
//	static void InitConstructor(int size, T* memory) { new(memory) T(size); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

	static T& assignmentOperator(const T& other, T* v) { (*v) = other; return *v; }

	template<typename Value>
	static T& assignmentOperatorValue(const Value& value, T* v) { (*v) = value; return *v; }

//	static void assign(int count, const V& value, T* v) { v->assign(count, value); }
	static int which(T* v) { return v->which(); }

	template<typename Value>
	static Value& get(T& v) { return boost::get<Value>(v); }

//	template<typename Value>
//	static const Value& getConst(const T& v) { return boost::get<Value>(v); }
};


/**
 * Register our variant bindings.
 */
template<typename VariantBase, typename T>
void registerVariantBindingsRecursive(scripting::IScriptingEngine* scriptingEngine, const std::string& name, std::list<std::string> types, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
}

/**
 * Register our variant bindings.
 */
template<typename VariantBase, typename T, typename Value, typename ... V>
void registerVariantBindingsRecursive(scripting::IScriptingEngine* scriptingEngine, const std::string& name, std::list<std::string> types, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	{
		const auto& type = types.front();

		scriptingEngine->registerGlobalFunction(type + "& get" + type + "(" + name + "& in)", asFUNCTION(VariantBase::template get<Value>), asCALL_CDECL);
//		scriptingEngine->registerGlobalFunction("const " + type + "& get" + type + "(const " + name + "& in)", asFUNCTION(VariantBase::getConst<Value>), asCALL_CDECL);

		scriptingEngine->registerObjectMethod(name.c_str(), name + "& opAssign(const " + type + "& in)", asFUNCTION(VariantBase::template assignmentOperatorValue<Value>), asCALL_CDECL_OBJLAST);
	}

	types.pop_front();
	registerVariantBindingsRecursive<VariantBase, T, V...>(scriptingEngine, name, types, objectTypeFlags);
}

/**
 * Register our variant bindings.
 */
template<typename ... V>
void registerVariantBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::list<std::string>& types, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{

	typedef VariantRegisterHelper<boost::variant<V...>, V...> VariantBase;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(boost::variant<V...>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<boost::variant<V...>>());

	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VariantBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(VariantBase::CopyConstructor), asCALL_CDECL_OBJLAST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(VariantBase::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VariantBase::DefaultDestructor), asCALL_CDECL_OBJLAST);

	scriptingEngine->registerObjectMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asFUNCTION(VariantBase::assignmentOperator), asCALL_CDECL_OBJLAST);

	scriptingEngine->registerObjectMethod(name.c_str(), "int which() const", asFUNCTION(VariantBase::which), asCALL_CDECL_OBJLAST);

	registerVariantBindingsRecursive<VariantBase, boost::variant<V...>, V...>(scriptingEngine, name, types, objectTypeFlags);
}

template<typename T>
class HandleRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }

	static void InitConstructor(T* memory, const uint64 id) { new(memory) T(id); }

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
    scriptingEngine->debugger()->registerToStringCallback(name.c_str(), scriptingEngineDebuggerToStringCallback<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
	scriptingEngine->registerClassMethod(name.c_str(), "uint64 id() const", asMETHODPR(T, id, () const, uint64));
	scriptingEngine->registerClassMethod(name.c_str(), "uint32 index() const", asMETHODPR(T, index, () const, uint32));
	scriptingEngine->registerClassMethod(name.c_str(), "uint32 version() const", asMETHODPR(T, version, () const, uint32));
	scriptingEngine->registerClassMethod(name.c_str(), "bool valid() const", asMETHODPR(T, valid, () const, bool));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}


template<typename T>
class PointerHandleRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }

	static void InitConstructor(T* memory, void* object) { new(memory) T(object); }

	static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
};

/**
 * Register our Pointer Handle bindings.
 */
template<typename T>
void registerPointerHandleBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
	typedef PointerHandleRegisterHelper<T> HandleBase;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(ref@)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
//	scriptingEngine->registerClassMethod(name.c_str(), scriptTypeName + "@ get()", asMETHODPR(T, get, (), void*));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

/**
 * Register our Pointer Handle bindings.
 */
template<typename T>
void registerPointerHandleBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& scriptTypeName)
{
	typedef PointerHandleRegisterHelper<T> HandleBase;

	scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<T>());
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(ref@)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
//	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
	scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
	scriptingEngine->registerClassMethod(name.c_str(), scriptTypeName + "@+ get()", asMETHODPR(T, get, (), void*));
	scriptingEngine->registerClassMethod(name.c_str(), "const " + scriptTypeName + "@+ get() const", asMETHODPR(T, get, () const, void*));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
	scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

template<class A, class B>
B* refCast(A* a)
{
    if (!a) return nullptr;

    return dynamic_cast<B*>(a);
    /*
    B* b = dynamic_cast<B*>(a);
    if( b != 0 )
    {
        // Since the cast was made, we need to increase the ref counter for the returned handle
        b->addref();
    }
    return b;
    */
}

template<class A, class B>
B valueCast(A& a)
{
    return static_cast<B>(a);
}

}

#endif /* BINDINGDELEGATEUTILITIES_H_ */
