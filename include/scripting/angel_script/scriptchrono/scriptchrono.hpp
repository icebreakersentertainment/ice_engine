#ifndef SCRIPT_CHRONO_H_
#define SCRIPT_CHRONO_H_

#include <new>
#include <chrono>

#include <angelscript.h>

BEGIN_AS_NAMESPACE

namespace stdchronoseconds
{
void DefaultConstructor(void* memory) { new(memory) std::chrono::seconds(); }
void CopyConstructor(const std::chrono::seconds& other, void* memory) { new(memory) std::chrono::seconds(other); }
//void DefaultDestructor(void* memory) { ((std::chrono::seconds*)memory)->~std::chrono::seconds(); }
void InitConstructor(int s, void* memory) { new(memory) std::chrono::seconds(s); }

static std::chrono::seconds operator+(const std::chrono::seconds& a, const std::chrono::seconds& b) { return std::chrono::seconds(a + b); }
static std::chrono::seconds operator-(const std::chrono::seconds& a, const std::chrono::seconds& b) { return std::chrono::seconds(a - b); }
//static std::chrono::seconds operator*(const std::chrono::seconds& a, const std::chrono::seconds& b) { return std::chrono::seconds(a * b); }
//static std::chrono::seconds operator/(const std::chrono::seconds& a, const std::chrono::seconds& b) { return std::chrono::seconds(a / b); }
static bool operator==(const std::chrono::seconds& a, const std::chrono::seconds& b) { return (a == b); }
}

/**
 * Register our chrono bindings.
 */
void RegisterChronoBindings(asIScriptEngine* engine)
{
	int r = 0;
	
	// std::chrono::seconds
	r = engine->SetDefaultNamespace("chrono"); assert( r >= 0 );
	
	r = engine->RegisterObjectType("seconds", sizeof(std::chrono::seconds), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<std::chrono::seconds>()); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("chrono::seconds", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(stdchronoseconds::DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("chrono::seconds", asBEHAVE_CONSTRUCT, "void f(const chrono::seconds &in)", asFUNCTION(stdchronoseconds::CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("chrono::seconds", asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(stdchronoseconds::InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectBehaviour("chrono::seconds", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(stdchronoseconds::DefaultDestructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds opAdd_r(const chrono::seconds& in) const", asFUNCTIONPR(stdchronoseconds::operator+, (const std::chrono::seconds&, const std::chrono::seconds&), std::chrono::seconds), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opAddAssign(const chrono::seconds& in)", asMETHODPR(std::chrono::seconds, operator+=, (const std::chrono::seconds&), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds opSub_r(const chrono::seconds& in) const", asFUNCTIONPR(stdchronoseconds::operator-, (const std::chrono::seconds&, const std::chrono::seconds&), std::chrono::seconds), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opSubAssign(const chrono::seconds &in)", asMETHODPR(std::chrono::seconds, operator-=, (const std::chrono::seconds&), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	//r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds opMul_r(const chrono::seconds& in) const", asFUNCTIONPR(stdchronoseconds::operator*, (const std::chrono::seconds&, const std::chrono::seconds&), std::chrono::seconds), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	
	//r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opMulAssign(int)", asMETHODPR(std::chrono::seconds, operator*=, (int), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	//r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds opDiv_r(const chrono::seconds& in) const", asFUNCTIONPR(stdchronoseconds::operator/, (const std::chrono::seconds&, const std::chrono::seconds&), std::chrono::seconds), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opDivAssign(int)", asMETHODPR(std::chrono::seconds, operator/=, (int), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opPreInc()", asMETHODPR(std::chrono::seconds, operator++, (), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "chrono::seconds &opPreDec()", asMETHODPR(std::chrono::seconds, operator--, (), std::chrono::seconds&), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("chrono::seconds", "bool opEquals(const chrono::seconds &in) const", asFUNCTIONPR(stdchronoseconds::operator==, (const std::chrono::seconds&, const std::chrono::seconds&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	
	r = engine->SetDefaultNamespace(""); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_CHRONO_H_ */
