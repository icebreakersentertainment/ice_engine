#ifndef SCRIPT_GLM_H_
#define SCRIPT_GLM_H_

#include <new>

#include <angelscript/angelscript.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

BEGIN_AS_NAMESPACE

namespace glmvec2
{
void DefaultConstructor(void* memory) { new(memory) glm::vec2(); }
void CopyConstructor(const glm::vec2& other, void* memory) { new(memory) glm::vec2(other); }
//void DefaultDestructor(void* memory) { ((glm::vec2*)memory)->~glm::vec2(); }
void InitConstructor(float x, float y, void* memory) { new(memory) glm::vec2(x,y); }

static glm::vec2 operator+(const glm::vec2& a, const glm::vec2& b) { return glm::vec2(a.x + b.x, a.y + b.y); }
static glm::vec2 operator-(const glm::vec2& a, const glm::vec2& b) { return glm::vec2(a.x - b.x, a.y - b.y); }
static glm::vec2 operator*(const glm::vec2& a, const glm::vec2& b) { return glm::vec2(a.x * b.x, a.y * b.y); }
static glm::vec2 operator/(const glm::vec2& a, const glm::vec2& b) { return glm::vec2(a.x / b.x, a.y / b.y); }
static bool operator==(const glm::vec2& a, const glm::vec2& b) { return (a.x == b.x && a.y == b.y); }
}

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

// glm::quat * glm::vec3
static glm::vec3 TimesQuat(const glm::quat& a, const glm::vec3& b) { return a * b; }
}

namespace glmvec4
{
void DefaultConstructor(void* memory) { new(memory) glm::vec4(); }
void CopyConstructor(const glm::vec4& other, void* memory) { new(memory) glm::vec4(other); }
//void DefaultDestructor(void* memory) { ((glm::vec4*)memory)->~glm::vec4(); }
void InitConstructor(float x, float y, float z, float w, void* memory) { new(memory) glm::vec4(x,y,z,w); }

static glm::vec4 operator+(const glm::vec4& a, const glm::vec4& b) { return glm::vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static glm::vec4 operator-(const glm::vec4& a, const glm::vec4& b) { return glm::vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static glm::vec4 operator*(const glm::vec4& a, const glm::vec4& b) { return glm::vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static glm::vec4 operator/(const glm::vec4& a, const glm::vec4& b) { return glm::vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static bool operator==(const glm::vec4& a, const glm::vec4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }

// glm::mat4 * glm::vec4
static glm::vec4 TimesMat4(const glm::mat4& a, const glm::vec4& b) { return a * b; }
}

namespace glmivec2
{
void DefaultConstructor(void* memory) { new(memory) glm::ivec2(); }
void CopyConstructor(const glm::ivec2& other, void* memory) { new(memory) glm::ivec2(other); }
//void DefaultDestructor(void* memory) { ((glm::ivec2*)memory)->~glm::ivec2(); }
void InitConstructor(int x, int y, void* memory) { new(memory) glm::ivec2(x,y); }

static glm::ivec2 operator+(const glm::ivec2& a, const glm::ivec2& b) { return glm::ivec2(a.x + b.x, a.y + b.y); }
static glm::ivec2 operator-(const glm::ivec2& a, const glm::ivec2& b) { return glm::ivec2(a.x - b.x, a.y - b.y); }
static glm::ivec2 operator*(const glm::ivec2& a, const glm::ivec2& b) { return glm::ivec2(a.x * b.x, a.y * b.y); }
static glm::ivec2 operator/(const glm::ivec2& a, const glm::ivec2& b) { return glm::ivec2(a.x / b.x, a.y / b.y); }
static bool operator==(const glm::ivec2& a, const glm::ivec2& b) { return (a.x == b.x && a.y == b.y); }
}

namespace glmivec3
{
void DefaultConstructor(void* memory) { new(memory) glm::ivec3(); }
void CopyConstructor(const glm::ivec3& other, void* memory) { new(memory) glm::ivec3(other); }
//void DefaultDestructor(void* memory) { ((glm::ivec3*)memory)->~glm::ivec3(); }
void InitConstructor(int x, int y, int z, void* memory) { new(memory) glm::ivec3(x,y,z); }

static glm::ivec3 operator+(const glm::ivec3& a, const glm::ivec3& b) { return glm::ivec3(a.x + b.x, a.y + b.y, a.z + b.z); }
static glm::ivec3 operator-(const glm::ivec3& a, const glm::ivec3& b) { return glm::ivec3(a.x - b.x, a.y - b.y, a.z - b.z); }
static glm::ivec3 operator*(const glm::ivec3& a, const glm::ivec3& b) { return glm::ivec3(a.x * b.x, a.y * b.y, a.z * b.z); }
static glm::ivec3 operator/(const glm::ivec3& a, const glm::ivec3& b) { return glm::ivec3(a.x / b.x, a.y / b.y, a.z / b.z); }
static bool operator==(const glm::ivec3& a, const glm::ivec3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }
}

namespace glmivec4
{
void DefaultConstructor(void* memory) { new(memory) glm::ivec4(); }
void CopyConstructor(const glm::ivec4& other, void* memory) { new(memory) glm::ivec4(other); }
//void DefaultDestructor(void* memory) { ((glm::ivec4*)memory)->~glm::ivec4(); }
void InitConstructor(int x, int y, int z, int w, void* memory) { new(memory) glm::ivec4(x,y,z,w); }

static glm::ivec4 operator+(const glm::ivec4& a, const glm::ivec4& b) { return glm::ivec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static glm::ivec4 operator-(const glm::ivec4& a, const glm::ivec4& b) { return glm::ivec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static glm::ivec4 operator*(const glm::ivec4& a, const glm::ivec4& b) { return glm::ivec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static glm::ivec4 operator/(const glm::ivec4& a, const glm::ivec4& b) { return glm::ivec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static bool operator==(const glm::ivec4& a, const glm::ivec4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
}

namespace glmuvec2
{
void DefaultConstructor(void* memory) { new(memory) glm::uvec2(); }
void CopyConstructor(const glm::uvec2& other, void* memory) { new(memory) glm::uvec2(other); }
//void DefaultDestructor(void* memory) { ((glm::uvec2*)memory)->~glm::uvec2(); }
void InitConstructor(int x, int y, void* memory) { new(memory) glm::uvec2(x,y); }

static glm::uvec2 operator+(const glm::uvec2& a, const glm::uvec2& b) { return glm::uvec2(a.x + b.x, a.y + b.y); }
static glm::uvec2 operator-(const glm::uvec2& a, const glm::uvec2& b) { return glm::uvec2(a.x - b.x, a.y - b.y); }
static glm::uvec2 operator*(const glm::uvec2& a, const glm::uvec2& b) { return glm::uvec2(a.x * b.x, a.y * b.y); }
static glm::uvec2 operator/(const glm::uvec2& a, const glm::uvec2& b) { return glm::uvec2(a.x / b.x, a.y / b.y); }
static bool operator==(const glm::uvec2& a, const glm::uvec2& b) { return (a.x == b.x && a.y == b.y); }
}

namespace glmuvec3
{
void DefaultConstructor(void* memory) { new(memory) glm::uvec3(); }
void CopyConstructor(const glm::uvec3& other, void* memory) { new(memory) glm::uvec3(other); }
//void DefaultDestructor(void* memory) { ((glm::uvec3*)memory)->~glm::uvec3(); }
void InitConstructor(int x, int y, int z, void* memory) { new(memory) glm::uvec3(x,y,z); }

static glm::uvec3 operator+(const glm::uvec3& a, const glm::uvec3& b) { return glm::uvec3(a.x + b.x, a.y + b.y, a.z + b.z); }
static glm::uvec3 operator-(const glm::uvec3& a, const glm::uvec3& b) { return glm::uvec3(a.x - b.x, a.y - b.y, a.z - b.z); }
static glm::uvec3 operator*(const glm::uvec3& a, const glm::uvec3& b) { return glm::uvec3(a.x * b.x, a.y * b.y, a.z * b.z); }
static glm::uvec3 operator/(const glm::uvec3& a, const glm::uvec3& b) { return glm::uvec3(a.x / b.x, a.y / b.y, a.z / b.z); }
static bool operator==(const glm::uvec3& a, const glm::uvec3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }
}

namespace glmuvec4
{
void DefaultConstructor(void* memory) { new(memory) glm::uvec4(); }
void CopyConstructor(const glm::uvec4& other, void* memory) { new(memory) glm::uvec4(other); }
//void DefaultDestructor(void* memory) { ((glm::uvec4*)memory)->~glm::uvec4(); }
void InitConstructor(int x, int y, int z, int w, void* memory) { new(memory) glm::uvec4(x,y,z,w); }

static glm::uvec4 operator+(const glm::uvec4& a, const glm::uvec4& b) { return glm::uvec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static glm::uvec4 operator-(const glm::uvec4& a, const glm::uvec4& b) { return glm::uvec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static glm::uvec4 operator*(const glm::uvec4& a, const glm::uvec4& b) { return glm::uvec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static glm::uvec4 operator/(const glm::uvec4& a, const glm::uvec4& b) { return glm::uvec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static bool operator==(const glm::uvec4& a, const glm::uvec4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
}

namespace glmmat4
{
void DefaultConstructor(void* memory) { new(memory) glm::mat4(); }
void CopyConstructor(const glm::mat4& other, void* memory) { new(memory) glm::mat4(other); }
//void DefaultDestructor(void* memory) { ((glm::mat4*)memory)->~glm::mat4(); }
void InitConstructor(float x, void* memory) { new(memory) glm::mat4(x); }

//static glm::mat4 operator+(const glm::mat4& a, const glm::mat4& b) { return glm::mat4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
//static glm::mat4 operator-(const glm::mat4& a, const glm::mat4& b) { return glm::mat4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
//static glm::mat4 operator*(const glm::mat4& a, const glm::mat4& b) { return glm::mat4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
//static glm::mat4 operator/(const glm::mat4& a, const glm::mat4& b) { return glm::mat4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
//static bool operator==(const glm::mat4& a, const glm::mat4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
}

namespace glmquat
{
void DefaultConstructor(void* memory) { new(memory) glm::quat(); }
void CopyConstructor(const glm::quat& other, void* memory) { new(memory) glm::quat(other); }
//void DefaultDestructor(void* memory) { ((glm::quat*)memory)->~glm::quat(); }
void InitConstructor(float x, float y, float z, float w, void* memory) { new(memory) glm::quat(x,y,z,w); }

static glm::quat operator+(const glm::quat& a, const glm::quat& b) { return glm::quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static glm::quat operator-(const glm::quat& a, const glm::quat& b) { return glm::quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static glm::quat operator*(const glm::quat& a, const glm::quat& b) { return glm::quat(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static glm::quat operator/(const glm::quat& a, const glm::quat& b) { return glm::quat(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static bool operator==(const glm::quat& a, const glm::quat& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }

// glm::vec3 * glm::quat
static glm::vec3 TimesVec3(const glm::vec3& a, const glm::quat& b) { return a * b; }
}


/**
 * Register our glm bindings.
 */
void RegisterGlmBindings(asIScriptEngine* engine)
{
	int r = 0;
	
	// glm::vec2
	r = engine->RegisterObjectType("vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::vec2>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2, y)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmvec2::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(const vec2 &in)", asFUNCTION(glmvec2::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(glmvec2::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("vec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmvec2::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("vec2", "vec2 opAdd_r(const vec2& in) const", asFUNCTIONPR(glmvec2::operator+, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opAddAssign(const vec2& in)", asMETHODPR(glm::vec2, operator+=, (const glm::vec2&), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 opSub_r(const vec2& in) const", asFUNCTIONPR(glmvec2::operator-, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opSubAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator-=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 opMul_r(const vec2& in) const", asFUNCTIONPR(glmvec2::operator*, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opMulAssign(float)", asMETHODPR(glm::vec2, operator*=, (float), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 opDiv_r(const vec2& in) const", asFUNCTIONPR(glmvec2::operator/, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opDivAssign(float)", asMETHODPR(glm::vec2, operator/=, (float), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opPreInc()", asMETHODPR(glm::vec2, operator++, (), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "vec2 &opPreDec()", asMETHODPR(glm::vec2, operator--, (), glm::vec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec2", "bool opEquals(const vec2 &in) const", asFUNCTIONPR(glmvec2::operator==, (const glm::vec2&, const glm::vec2&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
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
	
	// glm::vec4
	r = engine->RegisterObjectType("vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::vec4>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4, z)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4, w)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmvec4::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(const vec4 &in)", asFUNCTION(glmvec4::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(glmvec4::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("vec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmvec4::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("vec4", "vec4 opAdd_r(const vec4& in) const", asFUNCTIONPR(glmvec4::operator+, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opAddAssign(const vec4& in)", asMETHODPR(glm::vec4, operator+=, (const glm::vec4&), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 opSub_r(const vec4& in) const", asFUNCTIONPR(glmvec4::operator-, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opSubAssign(const vec4 &in)", asMETHODPR(glm::vec4, operator-=, (const glm::vec4 &), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 opMul_r(const vec4& in) const", asFUNCTIONPR(glmvec4::operator*, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opMulAssign(float)", asMETHODPR(glm::vec4, operator*=, (float), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 opDiv_r(const vec4& in) const", asFUNCTIONPR(glmvec4::operator/, (const glm::vec4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opDivAssign(float)", asMETHODPR(glm::vec4, operator/=, (float), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opPreInc()", asMETHODPR(glm::vec4, operator++, (), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "vec4 &opPreDec()", asMETHODPR(glm::vec4, operator--, (), glm::vec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("vec4", "bool opEquals(const vec4 &in) const", asFUNCTIONPR(glmvec4::operator==, (const glm::vec4&, const glm::vec4&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::ivec2
	r = engine->RegisterObjectType("ivec2", sizeof(glm::ivec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::ivec2>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec2", "int x", asOFFSET(glm::ivec2, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec2", "int y", asOFFSET(glm::ivec2, y)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmivec2::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec2", asBEHAVE_CONSTRUCT, "void f(const ivec2 &in)", asFUNCTION(glmivec2::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec2", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(glmivec2::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("ivec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmivec2::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("ivec2", "ivec2 opAdd_r(const ivec2& in) const", asFUNCTIONPR(glmivec2::operator+, (const glm::ivec2&, const glm::ivec2&), glm::ivec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opAddAssign(const ivec2& in)", asMETHODPR(glm::ivec2, operator+=, (const glm::ivec2&), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 opSub_r(const ivec2& in) const", asFUNCTIONPR(glmivec2::operator-, (const glm::ivec2&, const glm::ivec2&), glm::ivec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opSubAssign(const ivec2 &in)", asMETHODPR(glm::ivec2, operator-=, (const glm::ivec2 &), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 opMul_r(const ivec2& in) const", asFUNCTIONPR(glmivec2::operator*, (const glm::ivec2&, const glm::ivec2&), glm::ivec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opMulAssign(int)", asMETHODPR(glm::ivec2, operator*=, (int), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 opDiv_r(const ivec2& in) const", asFUNCTIONPR(glmivec2::operator/, (const glm::ivec2&, const glm::ivec2&), glm::ivec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opDivAssign(int)", asMETHODPR(glm::ivec2, operator/=, (int), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opPreInc()", asMETHODPR(glm::ivec2, operator++, (), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "ivec2 &opPreDec()", asMETHODPR(glm::ivec2, operator--, (), glm::ivec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec2", "bool opEquals(const ivec2 &in) const", asFUNCTIONPR(glmivec2::operator==, (const glm::ivec2&, const glm::ivec2&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::ivec3
	r = engine->RegisterObjectType("ivec3", sizeof(glm::ivec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::ivec3>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec3", "int x", asOFFSET(glm::ivec3, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec3", "int y", asOFFSET(glm::ivec3, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec3", "int z", asOFFSET(glm::ivec3, z)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmivec3::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec3", asBEHAVE_CONSTRUCT, "void f(const ivec3 &in)", asFUNCTION(glmivec3::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec3", asBEHAVE_CONSTRUCT, "void f(int, int, int)", asFUNCTION(glmivec3::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("ivec3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmivec3::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("ivec3", "ivec3 opAdd_r(const ivec3& in) const", asFUNCTIONPR(glmivec3::operator+, (const glm::ivec3&, const glm::ivec3&), glm::ivec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opAddAssign(const ivec3& in)", asMETHODPR(glm::ivec3, operator+=, (const glm::ivec3&), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 opSub_r(const ivec3& in) const", asFUNCTIONPR(glmivec3::operator-, (const glm::ivec3&, const glm::ivec3&), glm::ivec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opSubAssign(const ivec3 &in)", asMETHODPR(glm::ivec3, operator-=, (const glm::ivec3 &), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 opMul_r(const ivec3& in) const", asFUNCTIONPR(glmivec3::operator*, (const glm::ivec3&, const glm::ivec3&), glm::ivec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opMulAssign(int)", asMETHODPR(glm::ivec3, operator*=, (int), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 opDiv_r(const ivec3& in) const", asFUNCTIONPR(glmivec3::operator/, (const glm::ivec3&, const glm::ivec3&), glm::ivec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opDivAssign(int)", asMETHODPR(glm::ivec3, operator/=, (int), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opPreInc()", asMETHODPR(glm::ivec3, operator++, (), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "ivec3 &opPreDec()", asMETHODPR(glm::ivec3, operator--, (), glm::ivec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec3", "bool opEquals(const ivec3 &in) const", asFUNCTIONPR(glmivec3::operator==, (const glm::ivec3&, const glm::ivec3&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::ivec4
	r = engine->RegisterObjectType("ivec4", sizeof(glm::ivec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::ivec4>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec4", "int x", asOFFSET(glm::ivec4, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec4", "int y", asOFFSET(glm::ivec4, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec4", "int z", asOFFSET(glm::ivec4, z)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("ivec4", "int w", asOFFSET(glm::ivec4, w)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmivec4::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec4", asBEHAVE_CONSTRUCT, "void f(const ivec4 &in)", asFUNCTION(glmivec4::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("ivec4", asBEHAVE_CONSTRUCT, "void f(int, int, int, int)", asFUNCTION(glmivec4::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("ivec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmivec4::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("ivec4", "ivec4 opAdd_r(const ivec4& in) const", asFUNCTIONPR(glmivec4::operator+, (const glm::ivec4&, const glm::ivec4&), glm::ivec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opAddAssign(const ivec4& in)", asMETHODPR(glm::ivec4, operator+=, (const glm::ivec4&), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 opSub_r(const ivec4& in) const", asFUNCTIONPR(glmivec4::operator-, (const glm::ivec4&, const glm::ivec4&), glm::ivec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opSubAssign(const ivec4 &in)", asMETHODPR(glm::ivec4, operator-=, (const glm::ivec4 &), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 opMul_r(const ivec4& in) const", asFUNCTIONPR(glmivec4::operator*, (const glm::ivec4&, const glm::ivec4&), glm::ivec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opMulAssign(int)", asMETHODPR(glm::ivec4, operator*=, (int), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 opDiv_r(const ivec4& in) const", asFUNCTIONPR(glmivec4::operator/, (const glm::ivec4&, const glm::ivec4&), glm::ivec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opDivAssign(int)", asMETHODPR(glm::ivec4, operator/=, (int), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opPreInc()", asMETHODPR(glm::ivec4, operator++, (), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "ivec4 &opPreDec()", asMETHODPR(glm::ivec4, operator--, (), glm::ivec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("ivec4", "bool opEquals(const ivec4 &in) const", asFUNCTIONPR(glmivec4::operator==, (const glm::ivec4&, const glm::ivec4&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::uvec2
	r = engine->RegisterObjectType("uvec2", sizeof(glm::uvec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::uvec2>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec2", "int x", asOFFSET(glm::uvec2, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec2", "int y", asOFFSET(glm::uvec2, y)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmuvec2::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec2", asBEHAVE_CONSTRUCT, "void f(const uvec2 &in)", asFUNCTION(glmuvec2::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec2", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(glmuvec2::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("uvec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmuvec2::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("uvec2", "uvec2 opAdd_r(const uvec2& in) const", asFUNCTIONPR(glmuvec2::operator+, (const glm::uvec2&, const glm::uvec2&), glm::uvec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opAddAssign(const uvec2& in)", asMETHODPR(glm::uvec2, operator+=, (const glm::uvec2&), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 opSub_r(const uvec2& in) const", asFUNCTIONPR(glmuvec2::operator-, (const glm::uvec2&, const glm::uvec2&), glm::uvec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opSubAssign(const uvec2 &in)", asMETHODPR(glm::uvec2, operator-=, (const glm::uvec2 &), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 opMul_r(const uvec2& in) const", asFUNCTIONPR(glmuvec2::operator*, (const glm::uvec2&, const glm::uvec2&), glm::uvec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opMulAssign(int)", asMETHODPR(glm::uvec2, operator*=, (int), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 opDiv_r(const uvec2& in) const", asFUNCTIONPR(glmuvec2::operator/, (const glm::uvec2&, const glm::uvec2&), glm::uvec2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opDivAssign(int)", asMETHODPR(glm::uvec2, operator/=, (int), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opPreInc()", asMETHODPR(glm::uvec2, operator++, (), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "uvec2 &opPreDec()", asMETHODPR(glm::uvec2, operator--, (), glm::uvec2&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec2", "bool opEquals(const uvec2 &in) const", asFUNCTIONPR(glmuvec2::operator==, (const glm::uvec2&, const glm::uvec2&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::uvec3
	r = engine->RegisterObjectType("uvec3", sizeof(glm::uvec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::uvec3>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec3", "int x", asOFFSET(glm::uvec3, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec3", "int y", asOFFSET(glm::uvec3, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec3", "int z", asOFFSET(glm::uvec3, z)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmuvec3::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec3", asBEHAVE_CONSTRUCT, "void f(const uvec3 &in)", asFUNCTION(glmuvec3::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec3", asBEHAVE_CONSTRUCT, "void f(int, int, int)", asFUNCTION(glmuvec3::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("uvec3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmuvec3::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("uvec3", "uvec3 opAdd_r(const uvec3& in) const", asFUNCTIONPR(glmuvec3::operator+, (const glm::uvec3&, const glm::uvec3&), glm::uvec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opAddAssign(const uvec3& in)", asMETHODPR(glm::uvec3, operator+=, (const glm::uvec3&), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 opSub_r(const uvec3& in) const", asFUNCTIONPR(glmuvec3::operator-, (const glm::uvec3&, const glm::uvec3&), glm::uvec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opSubAssign(const uvec3 &in)", asMETHODPR(glm::uvec3, operator-=, (const glm::uvec3 &), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 opMul_r(const uvec3& in) const", asFUNCTIONPR(glmuvec3::operator*, (const glm::uvec3&, const glm::uvec3&), glm::uvec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opMulAssign(int)", asMETHODPR(glm::uvec3, operator*=, (int), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 opDiv_r(const uvec3& in) const", asFUNCTIONPR(glmuvec3::operator/, (const glm::uvec3&, const glm::uvec3&), glm::uvec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opDivAssign(int)", asMETHODPR(glm::uvec3, operator/=, (int), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opPreInc()", asMETHODPR(glm::uvec3, operator++, (), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "uvec3 &opPreDec()", asMETHODPR(glm::uvec3, operator--, (), glm::uvec3&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec3", "bool opEquals(const uvec3 &in) const", asFUNCTIONPR(glmuvec3::operator==, (const glm::uvec3&, const glm::uvec3&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::uvec4
	r = engine->RegisterObjectType("uvec4", sizeof(glm::uvec4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::uvec4>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec4", "int x", asOFFSET(glm::uvec4, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec4", "int y", asOFFSET(glm::uvec4, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec4", "int z", asOFFSET(glm::uvec4, z)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("uvec4", "int w", asOFFSET(glm::uvec4, w)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmuvec4::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec4", asBEHAVE_CONSTRUCT, "void f(const uvec4 &in)", asFUNCTION(glmuvec4::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("uvec4", asBEHAVE_CONSTRUCT, "void f(int, int, int, int)", asFUNCTION(glmuvec4::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("uvec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmuvec4::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("uvec4", "uvec4 opAdd_r(const uvec4& in) const", asFUNCTIONPR(glmuvec4::operator+, (const glm::uvec4&, const glm::uvec4&), glm::uvec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opAddAssign(const uvec4& in)", asMETHODPR(glm::uvec4, operator+=, (const glm::uvec4&), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 opSub_r(const uvec4& in) const", asFUNCTIONPR(glmuvec4::operator-, (const glm::uvec4&, const glm::uvec4&), glm::uvec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opSubAssign(const uvec4 &in)", asMETHODPR(glm::uvec4, operator-=, (const glm::uvec4 &), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 opMul_r(const uvec4& in) const", asFUNCTIONPR(glmuvec4::operator*, (const glm::uvec4&, const glm::uvec4&), glm::uvec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opMulAssign(int)", asMETHODPR(glm::uvec4, operator*=, (int), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 opDiv_r(const uvec4& in) const", asFUNCTIONPR(glmuvec4::operator/, (const glm::uvec4&, const glm::uvec4&), glm::uvec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opDivAssign(int)", asMETHODPR(glm::uvec4, operator/=, (int), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opPreInc()", asMETHODPR(glm::uvec4, operator++, (), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "uvec4 &opPreDec()", asMETHODPR(glm::uvec4, operator--, (), glm::uvec4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("uvec4", "bool opEquals(const uvec4 &in) const", asFUNCTIONPR(glmuvec4::operator==, (const glm::uvec4&, const glm::uvec4&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::mat4
	r = engine->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::mat4>()); assert( r >= 0 );
	//r = engine->RegisterObjectProperty("mat4", "int x", asOFFSET(glm::mat4, x)); assert( r >= 0 );
	//r = engine->RegisterObjectProperty("mat4", "int y", asOFFSET(glm::mat4, y)); assert( r >= 0 );
	//r = engine->RegisterObjectProperty("mat4", "int z", asOFFSET(glm::mat4, z)); assert( r >= 0 );
	//r = engine->RegisterObjectProperty("mat4", "int w", asOFFSET(glm::mat4, w)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmmat4::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f(const mat4 &in)", asFUNCTION(glmmat4::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("mat4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(glmmat4::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("mat4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmmat4::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	/*
	r = engine->RegisterObjectMethod("mat4", "mat4 opAdd_r(const mat4& in) const", asFUNCTIONPR(glmmat4::operator+, (const glm::mat4&, const glm::mat4&), glm::mat4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opAddAssign(const mat4& in)", asMETHODPR(glm::mat4, operator+=, (const glm::mat4&), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 opSub_r(const mat4& in) const", asFUNCTIONPR(glmmat4::operator-, (const glm::mat4&, const glm::mat4&), glm::mat4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opSubAssign(const mat4 &in)", asMETHODPR(glm::mat4, operator-=, (const glm::mat4 &), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 opMul_r(const mat4& in) const", asFUNCTIONPR(glmmat4::operator*, (const glm::mat4&, const glm::mat4&), glm::mat4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opMulAssign(float)", asMETHODPR(glm::mat4, operator*=, (int), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 opDiv_r(const mat4& in) const", asFUNCTIONPR(glmmat4::operator/, (const glm::mat4&, const glm::mat4&), glm::mat4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opDivAssign(float)", asMETHODPR(glm::mat4, operator/=, (int), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opPreInc()", asMETHODPR(glm::mat4, operator++, (), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "mat4 &opPreDec()", asMETHODPR(glm::mat4, operator--, (), glm::mat4&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("mat4", "bool opEquals(const mat4 &in) const", asFUNCTIONPR(glmmat4::operator==, (const glm::mat4&, const glm::mat4&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	*/
	// glm::quat
	r = engine->RegisterObjectType("quat", sizeof(glm::quat), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::quat>()); assert( r >= 0 );
	r = engine->RegisterObjectProperty("quat", "float x", asOFFSET(glm::quat, x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("quat", "float y", asOFFSET(glm::quat, y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("quat", "float z", asOFFSET(glm::quat, z)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("quat", "float w", asOFFSET(glm::quat, w)); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(glmquat::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f(const quat &in)", asFUNCTION(glmquat::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("quat", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(glmquat::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("quat", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(glmquat::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("quat", "quat opAdd_r(const quat& in) const", asFUNCTIONPR(glmquat::operator+, (const glm::quat&, const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat &opAddAssign(const quat& in)", asMETHODPR(glm::quat, operator+=, (const glm::quat&), glm::quat&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat opSub_r(const quat& in) const", asFUNCTIONPR(glmquat::operator-, (const glm::quat&, const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat &opSubAssign(const quat &in)", asMETHODPR(glm::quat, operator-=, (const glm::quat &), glm::quat&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat opMul_r(const quat& in) const", asFUNCTIONPR(glmquat::operator*, (const glm::quat&, const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat &opMulAssign(float)", asMETHODPR(glm::quat, operator*=, (float), glm::quat&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat opDiv_r(const quat& in) const", asFUNCTIONPR(glmquat::operator/, (const glm::quat&, const glm::quat&), glm::quat), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "quat &opDivAssign(float)", asMETHODPR(glm::quat, operator/=, (float), glm::quat&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("quat", "bool opEquals(const quat &in) const", asFUNCTIONPR(glmquat::operator==, (const glm::quat&, const glm::quat&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	// glm::vec3 * glm::quat
	r = engine->RegisterObjectMethod("quat", "vec3 opMul_r(const vec3& in) const", asFUNCTIONPR(glmquat::TimesVec3, (const glm::vec3&, const glm::quat&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	// glm::quat * glm::vec3
	r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(const quat& in) const", asFUNCTIONPR(glmvec3::TimesQuat, (const glm::quat&, const glm::vec3&), glm::vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	// glm::mat4 * glm::vec4
	r = engine->RegisterObjectMethod("vec4", "vec4 opMul_r(const mat4& in) const", asFUNCTIONPR(glmvec4::TimesMat4, (const glm::mat4&, const glm::vec4&), glm::vec4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	// glm functions
	r = engine->RegisterGlobalFunction("mat4 inverse(const mat4& in)", asFUNCTIONPR(glm::inverse, (const glm::mat4&), glm::mat4), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterGlobalFunction("vec3 normalize(const vec3& in)", asFUNCTIONPR(glm::normalize, (const glm::vec3&), glm::vec3), asCALL_CDECL); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_GLM_H_ */
