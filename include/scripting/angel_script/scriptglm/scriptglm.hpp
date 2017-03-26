#ifndef SCRIPT_GLM_H_
#define SCRIPT_GLM_H_

#include <new>

#include <angelscript/angelscript.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BEGIN_AS_NAMESPACE

namespace glmvec3
{
void DefaultConstructor(void* memory) { new(memory) glm::vec3(); }
void CopyConstructor(const glm::vec3& other, void* memory) { new(memory) glm::vec3(other); }
//void DefaultDestructor(void* memory) { ((glm::vec3*)memory)->~glm::vec3(); }
void InitConstructor(float x, float y, float z, void* memory) { new(memory) glm::vec3(x,y,z); }

static glm::vec3 operator+(const glm::vec3& a, const glm::vec3& b) { return glm::vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
static glm::vec3 operator-(const glm::vec3& a, const glm::vec3& b) { return glm::vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
static glm::vec3 operator*(const glm::vec3& a, const glm::vec3& b) { return glm::vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
static glm::vec3 operator/(const glm::vec3& a, const glm::vec3& b) { return glm::vec3(a.x / b.x, a.y / b.y, a.z / b.z); }
static bool operator==(const glm::vec3& a, const glm::vec3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }
}


/**
 * Register our glm bindings.
 */
void RegisterGlmBindings(asIScriptEngine* engine)
{
	int r = 0;
	
	// glm::vec3
	r = engine->RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::vec3>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmvec3::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)", asFUNCTION(glmvec3::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(glmvec3::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmvec3::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("vec3", "vec3 opAdd_r(const vec3& in) const", asFUNCTIONPR(glmvec3::operator+, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3& in)", asMETHODPR(glm::vec3, operator+=, (const glm::vec3&), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 opSub_r(const vec3& in) const", asFUNCTIONPR(glmvec3::operator-, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opSubAssign(const vec3 &in)", asMETHODPR(glm::vec3, operator-=, (const glm::vec3 &), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(const vec3& in) const", asFUNCTIONPR(glmvec3::operator*, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opMulAssign(float)", asMETHODPR(glm::vec3, operator*=, (float), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 opDiv_r(const vec3& in) const", asFUNCTIONPR(glmvec3::operator/, (const glm::vec3&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opDivAssign(float)", asMETHODPR(glm::vec3, operator/=, (float), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opPreInc()", asMETHODPR(glm::vec3, operator++, (), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "vec3 &opPreDec()", asMETHODPR(glm::vec3, operator--, (), glm::vec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec3", "bool opEquals(const vec3 &in) const", asFUNCTIONPR(glmvec3::operator==, (const glm::vec3&, const glm::vec3&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_GLM_H_ */
