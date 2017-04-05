#ifndef SCRIPT_VECTOR_H_
#define SCRIPT_VECTOR_H_

#include <new>
#include <vector>
#include <cassert>

#include <angelscript/angelscript.h>

BEGIN_AS_NAMESPACE

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
void RegisterVectorBindings(asIScriptEngine* engine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	
	typedef VectorRegisterHelper<std::vector<V>, V> VectorBase;
	
	int r = 0;
	
	r = engine->RegisterObjectType(name.c_str(), sizeof(std::vector<V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::vector<V>>()); assert( r >= 0 );

	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VectorBase::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(VectorBase::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(VectorBase::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VectorBase::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	auto assignFunctionString = std::string("void assign(int, const ") + type + "& in)";
	r = engine->RegisterObjectMethod(name.c_str(), assignFunctionString.c_str(), asFUNCTION(VectorBase::assign), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	r = engine->RegisterObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(VectorBase::assignmentOperator), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto pushBackFunctionString = "void push_back(" + type + "& in)";
	r = engine->RegisterObjectMethod(name.c_str(), pushBackFunctionString.c_str(), asFUNCTION(VectorBase::push_back), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void erase(int)", asFUNCTION(VectorBase::erase), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto insertFunctionString = "void insert(int, const " + type + "& in)";
	r = engine->RegisterObjectMethod(name.c_str(), insertFunctionString.c_str(), asFUNCTION(VectorBase::insert), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void pop_back()", asFUNCTION(VectorBase::pop_back), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "int size() const", asFUNCTION(VectorBase::size), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void resize(int)", asFUNCTION(VectorBase::resize), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto atFunctionString = type + "& at(int)";
	r = engine->RegisterObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto atConstFunctionString = std::string("const ") + type + "& at(int) const";
	r = engine->RegisterObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(VectorBase::at), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto indexFunctionString = type + "& opIndex(int)";
	r = engine->RegisterObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(VectorBase::index), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto indexConstFunctionString = std::string("const ") + type + "& opIndex(int) const";
	r = engine->RegisterObjectMethod(name.c_str(), indexConstFunctionString.c_str(), asFUNCTION(VectorBase::index), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto frontFunctionString = type + "& front()";
	r = engine->RegisterObjectMethod(name.c_str(), frontFunctionString.c_str(), asFUNCTION(VectorBase::front), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto frontConstFunctionString = std::string("const ") + type + "& front() const";
	r = engine->RegisterObjectMethod(name.c_str(), frontConstFunctionString.c_str(), asFUNCTION(VectorBase::front), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto backFunctionString = type + "& back()";
	r = engine->RegisterObjectMethod(name.c_str(), backFunctionString.c_str(), asFUNCTION(VectorBase::back), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto backConstFunctionString = std::string("const ") + type + "& back() const";
	r = engine->RegisterObjectMethod(name.c_str(), backConstFunctionString.c_str(), asFUNCTION(VectorBase::back), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void clear()", asFUNCTION(VectorBase::clear), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "bool empty() const", asFUNCTION(VectorBase::empty), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "int max_size() const", asFUNCTION(VectorBase::max_size), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void reserve(int)", asFUNCTION(VectorBase::reserve), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "int capacity() const", asFUNCTION(VectorBase::capacity), asCALL_CDECL_OBJLAST); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_VECTOR_H_ */
