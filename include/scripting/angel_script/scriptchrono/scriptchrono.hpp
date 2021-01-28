#ifndef SCRIPT_CHRONO_H_
#define SCRIPT_CHRONO_H_

#include <new>
#include <chrono>
#include <type_traits>

#include <angelscript.h>

BEGIN_AS_NAMESPACE

template<typename T>
void DefaultConstructor(void* memory) { new(memory) T(); }
template<typename T>
void CopyConstructor(const T& other, void* memory) { new(memory) T(other); }
template<typename T>
void DefaultDestructor(void* memory) { ((T*)memory)->~T(); }
template<typename T, typename V>
void InitConstructor(V v, void* memory) { new(memory) T(v); }

template<class T>
void registerDuration(asIScriptEngine* engine, const std::string& name, const std::string& rep, const std::string& period)
{
//    using DurationType = std::chrono::duration<Rep, Period>;
    using RepType = T::rep;

    engine->RegisterObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | (std::is_floating_point<T::rep>::value ? asOBJ_APP_CLASS_ALLFLOATS : asOBJ_APP_CLASS_ALLINTS) | asGetTypeTraits<T>());
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<T>), asCALL_CDECL_OBJLAST);
    std::string declaration = "void f(const " + rep + ")";
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, declaration.c_str(), asFUNCTION((InitConstructor<T, RepType>)), asCALL_CDECL_OBJLAST);
    declaration = "void f(const " + name + "& in)";
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, declaration.c_str(), asFUNCTION(CopyConstructor<T>), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<T>), asCALL_CDECL_OBJLAST);
    declaration = name + "& opAssign(const " + name + "& in)";
    engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asMETHODPR(T, operator=, (const T&), T&), asCALL_THISCALL);
//    declaration = "bool opEquals(const " + name + "& in) const";
//    engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asMETHODPR(T, operator==, (const T&) const, bool));
//    engine->RegisterObjectMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
    declaration = rep + " count() const";
    engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asMETHODPR(T, count, () const, T::rep), asCALL_THISCALL);
}

template<class A, class B>
B valueCast(A& a)
{
    return static_cast<B>(a);
}

/**
 * Register our chrono bindings.
 */
void RegisterChronoBindings(asIScriptEngine* engine)
{
	int r = 0;

	r = engine->SetDefaultNamespace("chrono"); assert( r >= 0 );

    // duration
    registerDuration<std::chrono::duration<float>>(engine, "durationFloat", "float", "ratio1");
    registerDuration<std::chrono::duration<int64_t>>(engine, "durationInt64", "int64", "ratio1");
    registerDuration<std::chrono::duration<int32_t>>(engine, "durationInt32", "int32", "ratio1");

//    using atto  = ratio<1, 1000000000000000000LL>;
//    using femto = ratio<1, 1000000000000000LL>;
//    using pico  = ratio<1, 1000000000000LL>;
//    using nano  = ratio<1, 1000000000>;
//    using micro = ratio<1, 1000000>;
//    using milli = ratio<1, 1000>;
//    using centi = ratio<1, 100>;
//    using deci  = ratio<1, 10>;
//    using deca  = ratio<10, 1>;
//    using hecto = ratio<100, 1>;
//    using kilo  = ratio<1000, 1>;
//    using mega  = ratio<1000000, 1>;
//    using giga  = ratio<1000000000, 1>;
//    using tera  = ratio<1000000000000LL, 1>;
//    using peta  = ratio<1000000000000000LL, 1>;
//    using exa   = ratio<1000000000000000000LL, 1>;
//
//    using nanoseconds  = duration<long long, nano>;
//    using microseconds = duration<long long, micro>;
//    using milliseconds = duration<long long, milli>;
//    using seconds      = duration<long long>;
//    using minutes      = duration<int, ratio<60>>;
//    using hours        = duration<int, ratio<3600>>;

    static_assert(std::is_same<std::chrono::nanoseconds::rep, int64_t>::value, "Representation for nanoseconds is not the same as int64_t");

    registerDuration<std::chrono::nanoseconds>(engine, "nanoseconds", "int64", "ratio1");

    engine->RegisterObjectMethod("durationInt64", "chrono::nanoseconds opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::nanoseconds::rep, std::chrono::nanoseconds::period>, std::chrono::nanoseconds>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt64", "chrono::nanoseconds opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::nanoseconds::rep, std::chrono::nanoseconds::period>, std::chrono::nanoseconds>)), asCALL_CDECL_OBJLAST);

    static_assert(std::is_same<std::chrono::microseconds::rep, int64_t>::value, "Representation for microseconds is not the same as int64_t");

    registerDuration<std::chrono::microseconds>(engine, "microseconds", "int64", "ratio1");

    engine->RegisterObjectMethod("durationInt64", "chrono::microseconds opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::microseconds::rep, std::chrono::microseconds::period>, std::chrono::microseconds>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt64", "chrono::microseconds opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::microseconds::rep, std::chrono::microseconds::period>, std::chrono::microseconds>)), asCALL_CDECL_OBJLAST);

    static_assert(std::is_same<std::chrono::milliseconds::rep, int64_t>::value, "Representation for milliseconds is not the same as int64_t");

    registerDuration<std::chrono::milliseconds>(engine, "milliseconds", "int64", "ratio1");

    engine->RegisterObjectMethod("durationInt64", "chrono::milliseconds opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::milliseconds::rep, std::chrono::milliseconds::period>, std::chrono::milliseconds>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt64", "chrono::milliseconds opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::milliseconds::rep, std::chrono::milliseconds::period>, std::chrono::milliseconds>)), asCALL_CDECL_OBJLAST);

    static_assert(std::is_same<std::chrono::seconds::rep, int64_t>::value, "Representation for seconds is not the same as int64_t");

    registerDuration<std::chrono::seconds>(engine, "seconds", "int64", "ratio1");

    engine->RegisterObjectMethod("durationInt64", "chrono::seconds opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::seconds::rep, std::chrono::seconds::period>, std::chrono::seconds>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt64", "chrono::seconds opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::seconds::rep, std::chrono::seconds::period>, std::chrono::seconds>)), asCALL_CDECL_OBJLAST);

    static_assert(std::is_same<std::chrono::minutes::rep, int32_t>::value, "Representation for minutes is not the same as int32_t");

    registerDuration<std::chrono::minutes>(engine, "minutes", "int32", "ratio1");

    engine->RegisterObjectMethod("durationInt32", "chrono::minutes opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::minutes::rep, std::chrono::minutes::period>, std::chrono::minutes>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt32", "chrono::minutes opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::minutes::rep, std::chrono::minutes::period>, std::chrono::minutes>)), asCALL_CDECL_OBJLAST);

    static_assert(std::is_same<std::chrono::hours::rep, int32_t>::value, "Representation for hours is not the same as int32_t");

    registerDuration<std::chrono::hours>(engine, "hours", "int32", "ratio1");

    engine->RegisterObjectMethod("durationInt32", "chrono::hours opConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::hours::rep, std::chrono::hours::period>, std::chrono::hours>)), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("durationInt32", "chrono::hours opImplConv()", asFUNCTION((valueCast<std::chrono::duration<std::chrono::hours::rep, std::chrono::hours::period>, std::chrono::hours>)), asCALL_CDECL_OBJLAST);

	r = engine->SetDefaultNamespace(""); assert( r >= 0 );
}

END_AS_NAMESPACE

#endif /* SCRIPT_CHRONO_H_ */
