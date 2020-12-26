#ifndef GLMVEC3_H_
#define GLMVEC3_H_

#include <angelscript.h>

#include <glm/glm.hpp>

BEGIN_AS_NAMESPACE

// This is not a complete 3D math library. It's only meant as a sample
// for registering value types. The application developers will most
// likely want to register their own math library instead.

struct Vec3
{
        Vec3();
        Vec3(const Vec3 &other);
        Vec3(float x, float y, float z);

        Vec3 &operator=(const Vec3 &other);
        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator*=(float scalar);
        Vec3 &operator/=(float scalar);

        float length() const;

        // Swizzle operators
        Vec3 get_xyz() const;
        void    set_xyz(const Vec3 &in);
        Vec3 get_yzx() const;
        void    set_yzx(const Vec3 &in);
        Vec3 get_zxy() const;
        void    set_zxy(const Vec3 &in);
        Vec3 get_zyx() const;
        void    set_zyx(const Vec3 &in);
        Vec3 get_yxz() const;
        void    set_yxz(const Vec3 &in);
        Vec3 get_xzy() const;
        void    set_xzy(const Vec3 &in);

        friend bool operator==(const Vec3 &a, const Vec3 &b);
        friend bool operator!=(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator+(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator-(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator*(float s, const Vec3 &v);
        friend Vec3 operator*(const Vec3 &v, float s);
        friend Vec3 operator/(const Vec3 &v, float s);

        float x;
        float y;
        float z;
};

// This function will determine the configuration of the engine
// and use one of the two functions below to register the string type
void RegisterGlmBindings(asIScriptEngine *engine);

// Call this function to register the math functions using native calling conventions
void RegisterGlmBindings_Native(asIScriptEngine *engine);

// Use this one instead if native calling conventions
// are not supported on the target platform
void RegisterGlmBindings_Generic(asIScriptEngine *engine);

END_AS_NAMESPACE

#endif
