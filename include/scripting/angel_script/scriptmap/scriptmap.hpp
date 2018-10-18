#ifndef SCRIPT_MAP_H_
#define SCRIPT_MAP_H_

#include <new>
#include <map>
#include <cassert>

#include <angelscript/angelscript.h>

BEGIN_AS_NAMESPACE

template<typename T, typename K, typename V>
class MapRegisterHelper
{
public:
	static void DefaultConstructor(T* memory) { new(memory) T(); }
	static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
	static void InitConstructor(int size, T* memory) { new(memory) T(size); }
	static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
	
	static T& assignmentOperator(const T& other, T* m) { (*m) = other; return *m; }
	
	static int size(T* m) { return m->size(); }
	static V& at(const K& k, T* m) { return m->at(k); }
	static V& index(const K& k, T* m) { return (*m)[k]; }
	static void clear(T* m) { m->clear(); }
	static bool empty(T* m) { return m->empty(); }
	static int max_size(T* m) { return m->max_size(); }
};


/**
 * Register our map bindings.
 */
template<typename K, typename V>
void RegisterMapBindings(asIScriptEngine* engine, const std::string& name, const std::string& type, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{
	
	typedef MapRegisterHelper<std::map<K, V>, K, V> MapBase;
	
	int r = 0;
	
	r = engine->RegisterObjectType(name.c_str(), sizeof(std::map<K, V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::map<K, V>>()); assert( r >= 0 );

	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(MapBase::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto copyConstructorString = std::string("void f(const ") + name + "& in)";
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(MapBase::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(MapBase::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(MapBase::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
	r = engine->RegisterObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(MapBase::assignmentOperator), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "int size() const", asFUNCTION(MapBase::size), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto atFunctionString = type + "& at(int)";
	r = engine->RegisterObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(MapBase::at), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto atConstFunctionString = std::string("const ") + type + "& at(int) const";
	r = engine->RegisterObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(MapBase::at), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto indexFunctionString = type + "& opIndex(int)";
	r = engine->RegisterObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(MapBase::index), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	auto indexConstFunctionString = std::string("const ") + type + "& opIndex(int) const";
	r = engine->RegisterObjectMethod(name.c_str(), indexConstFunctionString.c_str(), asFUNCTION(MapBase::index), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "void clear()", asFUNCTION(MapBase::clear), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "bool empty() const", asFUNCTION(MapBase::empty), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod(name.c_str(), "int max_size() const", asFUNCTION(MapBase::max_size), asCALL_CDECL_OBJLAST); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_MAP_H_ */
