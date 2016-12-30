#include <assert.h>
#include <string.h> // strstr
#include <new> // new()
#include <math.h>

#include "Vec3.h"

#ifdef __BORLANDC__
// C++Builder doesn't define a non-standard "sqrtf" function but rather an overload of "sqrt"
// for float arguments.
inline float sqrtf (float x) { return sqrt (x); }
#endif

BEGIN_AS_NAMESPACE

Vec3::Vec3()
{
        x = 0;
        y = 0;
        z = 0;
}

Vec3::Vec3(const Vec3 &other)
{
        x = other.x;
        y = other.y;
        z = other.z;
}

Vec3::Vec3(float _x, float _y, float _z)
{
        x = _x;
        y = _y;
        z = _z;
}

bool operator==(const Vec3 &a, const Vec3 &b)
{
        return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

bool operator!=(const Vec3 &a, const Vec3 &b)
{
        return !(a == b);
}

Vec3 &Vec3::operator=(const Vec3 &other)
{
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other)
{
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
}

Vec3 &Vec3::operator*=(float s)
{
        x *= s;
        y *= s;
        z *= s;
        return *this;
}

Vec3 &Vec3::operator/=(float s)
{
        x /= s;
        y /= s;
        z /= s;
        return *this;
}

float Vec3::length() const
{
        return sqrtf(x*x + y*y + z*z);
}

Vec3 operator+(const Vec3 &a, const Vec3 &b)
{
        // Return a new object as a script handle
        Vec3 res(a.x + b.x, a.y + b.y, a.z + b.z);
        return res;
}

Vec3 operator-(const Vec3 &a, const Vec3 &b)
{
        // Return a new object as a script handle
        Vec3 res(a.x - b.x, a.y - b.y, a.z - b.z);
        return res;
}

Vec3 operator*(float s, const Vec3 &v)
{
        // Return a new object as a script handle
        Vec3 res(v.x * s, v.y * s, v.z * s);
        return res;
}

Vec3 operator*(const Vec3 &v, float s)
{
        // Return a new object as a script handle
        Vec3 res(v.x * s, v.y * s, v.z * s);
        return res;
}

Vec3 operator/(const Vec3 &v, float s)
{
        // Return a new object as a script handle
        Vec3 res(v.x / s, v.y / s, v.z / s);
        return res;
}

//-----------------------
// Swizzle operators
//-----------------------

Vec3 Vec3::get_xyz() const
{
        return Vec3(x,y,z);
}
Vec3 Vec3::get_yzx() const
{
        return Vec3(y,z,x);
}
Vec3 Vec3::get_zxy() const
{
        return Vec3(z,x,y);
}
Vec3 Vec3::get_zyx() const
{
        return Vec3(z,y,x);
}
Vec3 Vec3::get_yxz() const
{
        return Vec3(y,x,z);
}
Vec3 Vec3::get_xzy() const
{
        return Vec3(x,z,y);
}
void Vec3::set_xyz(const Vec3 &v)
{
        x = v.x; y = v.y; z = v.z;
}
void Vec3::set_yzx(const Vec3 &v)
{
        y = v.x; z = v.y; x = v.z;
}
void Vec3::set_zxy(const Vec3 &v)
{
        z = v.x; x = v.y; y = v.z;
}
void Vec3::set_zyx(const Vec3 &v)
{
        z = v.x; y = v.y; x = v.z;
}
void Vec3::set_yxz(const Vec3 &v)
{
        y = v.x; x = v.y; z = v.z;
}
void Vec3::set_xzy(const Vec3 &v)
{
        x = v.x; z = v.y; y = v.z;
}

//-----------------------
// AngelScript functions
//-----------------------

static void Vec3DefaultConstructor(Vec3 *self)
{
        new(self) Vec3();
}

static void Vec3CopyConstructor(const Vec3 &other, Vec3 *self)
{
        new(self) Vec3(other);
}

static void Vec3InitConstructor(float x, float y, float z, Vec3 *self)
{
        new(self) Vec3(x,y,z);
}

//-----------------------
// Generic calling convention
//-----------------------

static void Vec3DefaultConstructor_Generic(asIScriptGeneric *gen)
{
        Vec3 *self = (Vec3*)gen->GetObject();
        new(self) Vec3();
}

static void Vec3CopyConstructor_Generic(asIScriptGeneric *gen)
{
        Vec3 *other = (Vec3*)gen->GetArgObject(0);
        Vec3 *self = (Vec3*)gen->GetObject();
        Vec3CopyConstructor(*other, self);
}

static void Vec3InitConstructor_Generic(asIScriptGeneric *gen)
{
        float x = gen->GetArgFloat(0);
        float y = gen->GetArgFloat(1);
        float z = gen->GetArgFloat(2);
        Vec3 *self = (Vec3*)gen->GetObject();
        Vec3InitConstructor(x,y,z,self);
}

static void Vec3Equal_Generic(asIScriptGeneric *gen)
{
        Vec3 *a = (Vec3*)gen->GetObject();
        Vec3 *b = (Vec3*)gen->GetArgAddress(0);
        bool r = *a == *b;
    *(bool*)gen->GetAddressOfReturnLocation() = r;
}

static void Vec3Length_Generic(asIScriptGeneric *gen)
{
        Vec3 *s = (Vec3*)gen->GetObject();
        gen->SetReturnFloat(s->length());
}

static void Vec3AddAssign_Generic(asIScriptGeneric *gen)
{
        Vec3 *a = (Vec3*)gen->GetArgAddress(0);
        Vec3 *thisPointer = (Vec3*)gen->GetObject();
        *thisPointer += *a;
        gen->SetReturnAddress(thisPointer);
}

static void Vec3SubAssign_Generic(asIScriptGeneric *gen)
{
        Vec3 *a = (Vec3*)gen->GetArgAddress(0);
        Vec3 *thisPointer = (Vec3*)gen->GetObject();
        *thisPointer -= *a;
        gen->SetReturnAddress(thisPointer);
}

static void Vec3MulAssign_Generic(asIScriptGeneric *gen)
{
        float s = gen->GetArgFloat(0);
        Vec3 *thisPointer = (Vec3*)gen->GetObject();
        *thisPointer *= s;
        gen->SetReturnAddress(thisPointer);
}

static void Vec3DivAssign_Generic(asIScriptGeneric *gen)
{
        float s = gen->GetArgFloat(0);
        Vec3 *thisPointer = (Vec3*)gen->GetObject();
        *thisPointer /= s;
        gen->SetReturnAddress(thisPointer);
}

static void Vec3Add_Generic(asIScriptGeneric *gen)
{
        Vec3 *a = (Vec3*)gen->GetObject();
        Vec3 *b = (Vec3*)gen->GetArgAddress(0);
        Vec3 res = *a + *b;
        gen->SetReturnObject(&res);
}

static void Vec3Sub_Generic(asIScriptGeneric *gen)
{
        Vec3 *a = (Vec3*)gen->GetObject();
        Vec3 *b = (Vec3*)gen->GetArgAddress(0);
        Vec3 res = *a - *b;
        gen->SetReturnObject(&res);
}

static void Vec3FloatMulVec3_Generic(asIScriptGeneric *gen)
{
        float s = gen->GetArgFloat(0);
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 res = s * *v;
        gen->SetReturnObject(&res);
}

static void Vec3Vec3MulFloat_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        float s = gen->GetArgFloat(0);
        Vec3 res = *v * s;
        gen->SetReturnObject(&res);
}

static void Vec3Vec3DivFloat_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        float s = gen->GetArgFloat(0);
        Vec3 res = *v / s;
        gen->SetReturnObject(&res);
}

static void Vec3_get_xyz_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_xyz();
}

static void Vec3_get_yzx_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_yzx();
}

static void Vec3_get_zxy_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_zxy();
}

static void Vec3_get_zyx_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_zyx();
}

static void Vec3_get_yxz_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_yxz();
}

static void Vec3_get_xzy_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        *(Vec3*)gen->GetAddressOfReturnLocation() = v->get_xzy();
}

static void Vec3_set_xyz_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_xyz(*i);
}

static void Vec3_set_yzx_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_yzx(*i);
}

static void Vec3_set_zxy_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_zxy(*i);
}

static void Vec3_set_zyx_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_zyx(*i);
}

static void Vec3_set_yxz_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_yxz(*i);
}

static void Vec3_set_xzy_Generic(asIScriptGeneric *gen)
{
        Vec3 *v = (Vec3*)gen->GetObject();
        Vec3 *i = *(Vec3**)gen->GetAddressOfArg(0);
        v->set_xzy(*i);
}

//--------------------------------
// Registration
//-------------------------------------

void RegisterGlmBindings_Native(asIScriptEngine *engine)
{
        int r;

        // Register the type
        r = engine->RegisterObjectType("vec3", sizeof(Vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert( r >= 0 );

        // Register the object properties
        r = engine->RegisterObjectProperty("vec3", "float x", offsetof(Vec3, x)); assert( r >= 0 );
        r = engine->RegisterObjectProperty("vec3", "float y", offsetof(Vec3, y)); assert( r >= 0 );
        r = engine->RegisterObjectProperty("vec3", "float z", offsetof(Vec3, z)); assert( r >= 0 );

        // Register the constructors
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT,  "void f()",                     asFUNCTION(Vec3DefaultConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT,  "void f(const vec3 &in)",    asFUNCTION(Vec3CopyConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT,  "void f(float, float, float)",  asFUNCTION(Vec3InitConstructor), asCALL_CDECL_OBJLAST); assert( r >= 0 );

        // Register the operator overloads
        r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3 &in)", asMETHODPR(Vec3, operator+=, (const Vec3 &), Vec3&), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opSubAssign(const vec3 &in)", asMETHODPR(Vec3, operator-=, (const Vec3 &), Vec3&), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opMulAssign(float)", asMETHODPR(Vec3, operator*=, (float), Vec3&), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opDivAssign(float)", asMETHODPR(Vec3, operator/=, (float), Vec3&), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "bool opEquals(const vec3 &in) const", asFUNCTIONPR(operator==, (const Vec3&, const Vec3&), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        /*
        r = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const", asFUNCTIONPR(operator+, (const Vec3&, const Vec3&), Vec3), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const", asFUNCTIONPR(operator-, (const Vec3&, const Vec3&), Vec3), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const", asFUNCTIONPR(operator*, (const Vec3&, float), Vec3), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const", asFUNCTIONPR(operator*, (float, const Vec3&), Vec3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opDiv(float) const", asFUNCTIONPR(operator/, (const Vec3&, float), Vec3), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		*/
		
        // Register the object methods
        r = engine->RegisterObjectMethod("vec3", "float length() const", asMETHOD(Vec3,length), asCALL_THISCALL); assert( r >= 0 );

        // Register the swizzle operators
        /*
        r = engine->RegisterObjectMethod("vec3", "vec3 get_xyz() const", asMETHOD(Vec3, get_xyz), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_yzx() const", asMETHOD(Vec3, get_yzx), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_zxy() const", asMETHOD(Vec3, get_zxy), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_zyx() const", asMETHOD(Vec3, get_zyx), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_yxz() const", asMETHOD(Vec3, get_yxz), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_xzy() const", asMETHOD(Vec3, get_xzy), asCALL_THISCALL); assert( r >= 0 );
        */
        r = engine->RegisterObjectMethod("vec3", "void set_xyz(const vec3 &in)", asMETHOD(Vec3, set_xyz), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_yzx(const vec3 &in)", asMETHOD(Vec3, set_yzx), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_zxy(const vec3 &in)", asMETHOD(Vec3, set_zxy), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_zyx(const vec3 &in)", asMETHOD(Vec3, set_zyx), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_yxz(const vec3 &in)", asMETHOD(Vec3, set_yxz), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_xzy(const vec3 &in)", asMETHOD(Vec3, set_xzy), asCALL_THISCALL); assert( r >= 0 );
}

void RegisterGlmBindings_Generic(asIScriptEngine *engine)
{
        int r;

        // Register the type
        r = engine->RegisterObjectType("vec3", sizeof(Vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CA); assert( r >= 0 );

        // Register the object properties
        r = engine->RegisterObjectProperty("vec3", "float x", offsetof(Vec3, x)); assert( r >= 0 );
        r = engine->RegisterObjectProperty("vec3", "float y", offsetof(Vec3, y)); assert( r >= 0 );
        r = engine->RegisterObjectProperty("vec3", "float z", offsetof(Vec3, z)); assert( r >= 0 );

        // Register the constructors
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()",                    asFUNCTION(Vec3DefaultConstructor_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(const vec3 &in)",   asFUNCTION(Vec3CopyConstructor_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(Vec3InitConstructor_Generic), asCALL_GENERIC); assert( r >= 0 );

        // Register the operator overloads
        r = engine->RegisterObjectMethod("vec3", "vec3 &opAddAssign(const vec3 &in)", asFUNCTION(Vec3AddAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opSubAssign(const vec3 &in)", asFUNCTION(Vec3SubAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opMulAssign(float)", asFUNCTION(Vec3MulAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 &opDivAssign(float)", asFUNCTION(Vec3DivAssign_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "bool opEquals(const vec3 &in) const", asFUNCTION(Vec3Equal_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opAdd(const vec3 &in) const", asFUNCTION(Vec3Add_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opSub(const vec3 &in) const", asFUNCTION(Vec3Sub_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opMul_r(float) const", asFUNCTION(Vec3FloatMulVec3_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opMul(float) const", asFUNCTION(Vec3Vec3MulFloat_Generic), asCALL_GENERIC); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 opDiv(float) const", asFUNCTION(Vec3Vec3DivFloat_Generic), asCALL_GENERIC); assert( r >= 0 );

        // Register the object methods
        r = engine->RegisterObjectMethod("vec3", "float length() const", asFUNCTION(Vec3Length_Generic), asCALL_GENERIC); assert( r >= 0 );

        // Register the swizzle operators
        r = engine->RegisterObjectMethod("vec3", "vec3 get_xyz() const", asFUNCTION(Vec3_get_xyz_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_yzx() const", asFUNCTION(Vec3_get_yzx_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_zxy() const", asFUNCTION(Vec3_get_zxy_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_zyx() const", asFUNCTION(Vec3_get_zyx_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_yxz() const", asFUNCTION(Vec3_get_yxz_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "vec3 get_xzy() const", asFUNCTION(Vec3_get_xzy_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_xyz(const vec3 &in)", asFUNCTION(Vec3_set_xyz_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_yzx(const vec3 &in)", asFUNCTION(Vec3_set_yzx_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_zxy(const vec3 &in)", asFUNCTION(Vec3_set_zxy_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_zyx(const vec3 &in)", asFUNCTION(Vec3_set_zyx_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_yxz(const vec3 &in)", asFUNCTION(Vec3_set_yxz_Generic), asCALL_THISCALL); assert( r >= 0 );
        r = engine->RegisterObjectMethod("vec3", "void set_xzy(const vec3 &in)", asFUNCTION(Vec3_set_xzy_Generic), asCALL_THISCALL); assert( r >= 0 );
}

void RegisterGlmBindings(asIScriptEngine *engine)
{
        if( strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
                RegisterGlmBindings_Generic(engine);
        else
                RegisterGlmBindings_Native(engine);
}

END_AS_NAMESPACE
