#ifndef SCRIPT_GLM_H_
#define SCRIPT_GLM_H_

#include <new>

#include <angelscript/angelscript.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BEGIN_AS_NAMESPACE

// Macro magic courtesy of http://stackoverflow.com/a/13842612/780281
#define SINGLE_ARG(...) __VA_ARGS__

#define GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES1(name) \
namespace glm##name \
{ \
template <class C> void DefaultConstructor(void* memory) { new(memory) C(); } \
template <class C> void CopyConstructor(const C& other, void* memory) { new(memory) C(other); } \
template <class C> void DefaultDestructor(void* memory) { ((C*)memory)->~C(); } \
}

#define GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES3(name, args, vars) \
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES1(name) \
namespace glm##name \
{ \
template <class C> void InitConstructor(args, void* memory) { new(memory) C(vars); } \
}

#define GLM_REGISTER_OBJECT(name) \
r = engine->RegisterObjectType(#name, sizeof(glm::name), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<glm::name>()); assert( r >= 0 );

#define GLM_REGISTER_PROPERTIES(name, type, var) \
r = engine->RegisterObjectProperty(#name, #type" "#var, asOFFSET(glm::name, var)); assert( r >= 0 );

#define GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR1(name) \
r = engine->RegisterObjectBehaviour(#name, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glm##name::DefaultConstructor<glm::name>), asCALL_CDECL_OBJLAST); assert( r >= 0 ); \
r = engine->RegisterObjectBehaviour(#name, asBEHAVE_CONSTRUCT, "void f(const "#name" &in)", asFUNCTION(glm##name::CopyConstructor<glm::name>), asCALL_CDECL_OBJLAST); assert( r >= 0 ); \
r = engine->RegisterObjectBehaviour(#name, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glm##name::DefaultDestructor<glm::name>), asCALL_CDECL_OBJLAST); assert( r >= 0 ); \

#define GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR2(name, paramTypesList) \
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR1(name) \
r = engine->RegisterObjectBehaviour(#name, asBEHAVE_CONSTRUCT, "void f"#paramTypesList, asFUNCTION(glm##name::InitConstructor<glm::name>), asCALL_CDECL_OBJLAST); assert( r >= 0 );

// Must register our constructor / destructor templates first
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES3(vec2, SINGLE_ARG(float x, float y), SINGLE_ARG(x,y));
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES3(vec3, SINGLE_ARG(float x, float y, float z), SINGLE_ARG(x,y,z));
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES3(vec4, SINGLE_ARG(float x, float y, float z, float w), SINGLE_ARG(x,y,z,w));

GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES1(mat2);
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES1(mat3);
GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR_TEMPLATES1(mat4);

namespace glmvec3
{
static glm::vec3 operator+(const glm::vec3& a, const glm::vec3& b)
{
	return glm::vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
}


/**
 * Register our glm bindings.
 */
void RegisterGlmBindings(asIScriptEngine* engine)
{
	int r = 0;
	
	GLM_REGISTER_OBJECT(vec2);
	GLM_REGISTER_OBJECT(vec3);
	GLM_REGISTER_OBJECT(vec4);
	
	GLM_REGISTER_OBJECT(mat2);
	GLM_REGISTER_OBJECT(mat3);
	GLM_REGISTER_OBJECT(mat4);
	
	// Register the object properties
	GLM_REGISTER_PROPERTIES(vec2, float, x);
	GLM_REGISTER_PROPERTIES(vec2, float, y);
	
	GLM_REGISTER_PROPERTIES(vec3, float, x);
	GLM_REGISTER_PROPERTIES(vec3, float, y);
	GLM_REGISTER_PROPERTIES(vec3, float, z);
	
	GLM_REGISTER_PROPERTIES(vec4, float, x);
	GLM_REGISTER_PROPERTIES(vec4, float, y);
	GLM_REGISTER_PROPERTIES(vec4, float, z);
	GLM_REGISTER_PROPERTIES(vec4, float, w);
	
	// Register the object constructor/destructor
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR2(vec2, (float, float));
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR2(vec3, (float, float, float));
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR2(vec4, (float, float, float, float));
	
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR1(mat2);
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR1(mat3);
	GLM_REGISTER_CONSTRUCTOR_DESTRUCTOR1(mat4);
	
	// testing
	engine->RegisterObjectMethod("vec3", "vec3 opAdd_r(const vec3& in) const", asFUNCTIONPR(glmvec3::operator+, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST);
	engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3& in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_GLM_H_ */
