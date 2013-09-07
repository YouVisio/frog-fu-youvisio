//
//  Vector.hpp
//  FrogFu
//
//  Created by Vladimir Bodurov on 12-08-05.
//  Copyright (c) 2012 Vladimir Bodurov. All rights reserved.
//

#ifndef FrogFu_Vector_hpp
#define FrogFu_Vector_hpp

#import <cmath>
#import <iostream>

const float Pi = 4 * std::atan(1.0f);
const float TwoPi = 2 * Pi;

using namespace std;

template <typename T>
struct Vector2 {
    Vector2() {}
    Vector2(T x, T y) : x(x), y(y) {}
    Vector2(const Vector2& v) : x(v.x), y(v.y) {}
    T Dot(const Vector2& v) const
    {
        return x * v.x + y * v.y;
    }
    Vector2 operator+(const Vector2& v) const
    {
        return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }
    Vector2 operator/(float s) const
    {
        return Vector2(x / s, y / s);
    }
    Vector2 operator*(float s) const
    {
        return Vector2(x * s, y * s);
    }
    Vector2 Step(Vector2 target, double progress){
        return Vector2(((target.x-x)*progress)+x, ((target.y-y)*progress)+y);
    }
    Vector2 Scale(Vector2 center, double scale){
        return Vector2(
                    (x - center.x) * scale + center.x,
                    (y - center.y) * scale + center.y
                    );
    }
    Vector2 DotOnTheWayTo(Vector2 next, double len){
        double a = atan2(next.x - x, next.y - y);
        double y1 = cos(a) * len + next.y;
        double x1 = sin(a) * len + next.x;
        return Vector2(x1, y1);
    }
    double LengthTo(Vector2 target){
        return sqrt(pow(target.x - x, 2) + pow(target.y - y, 2));
    }
    double AngleTo(Vector2 target){
        return atan2(x - target.x, y - target.y);
    }
    void Set(Vector2 v){
        x = v.x;
        y = v.y;
    }
    void Set(T xVal, T yVal){
        x = xVal;
        y = yVal;
    }
    void Shift(T xVal, T yVal){
        x += xVal;
        y += yVal;
    }
    void Normalize()
    {
        float s = 1.0f / Length();
        x *= s;
        y *= s;
    }
    float Distance(Vector2 other){
        return sqrt(pow((double)(other.x - x), 2.0) + pow((double)(other.y - y), 2.0));
    }
    Vector2 Rotate(Vector2 center, float radians)
    {
        float tmpX1 = x - center.x;
        float tmpY1 = y - center.y;
        float tmpX2 = tmpX1 * cos(radians) - tmpY1 * sin(radians);
        float tmpY2 = tmpY1 * cos(radians) + tmpX1 * sin(radians);
        x = tmpX2 + center.x;
        y = tmpY2 + center.y;
        return *this;
    }
    Vector2 GetRotated(Vector2 center, float radians)
    {
        float tmpX1 = x - center.x;
        float tmpY1 = y - center.y;
        float tmpX2 = tmpX1 * cos(radians) - tmpY1 * sin(radians);
        float tmpY2 = tmpY1 * cos(radians) + tmpX1 * sin(radians);
        return Vector2(tmpX2 + center.x, tmpY2 + center.y);
    }
    Vector2 Normalized() const
    {
        Vector2 v = *this;
        v.Normalize();
        return v;
    }
    T LengthSquared() const
    {
        return x * x + y * y;
    }
    T Length() const
    {
        return sqrt(LengthSquared());
    }
    operator Vector2<float>() const
    {
        return Vector2<float>(x, y);
    }
    bool operator==(const Vector2& v) const
    {
        return x == v.x && y == v.y;
    }
    Vector2 Lerp(float t, const Vector2& v) const
    {
        return Vector2(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t);
    }
    std::ostream& out(std::ostream& os) const{
        return os << "[x=" <<  x << " y=" << y << "]";
    }
    template <typename P>
    P* Write(P* pData)
    {
        Vector2* pVector = (Vector2*) pData;
        *pVector++ = *this;
        return (P*) pVector;
    }
    T x;
    T y;
};
template <typename T>
struct Vector3 {
    Vector3() {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    void Normalize()
    {
        float s = 1.0f / std::sqrt(x * x + y * y + z * z);
        x *= s;
        y *= s;
        z *= s;
    }
    Vector3 Normalized() const
    {
        Vector3 v = *this;
        v.Normalize();
        return v;
    }
    Vector3 Cross(const Vector3& v) const
    {
        return Vector3(y * v.z - z * v.y,
                       z * v.x - x * v.z,
                       x * v.y - y * v.x);
    }
    T Dot(const Vector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y,  z + v.z);
    }
    void operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    void operator-=(const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator/=(T s)
    {
        x /= s;
        y /= s;
        z /= s;
    }
    Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y,  z - v.z);
    }
    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }
    Vector3 operator*(T s) const
    {
        return Vector3(x * s, y * s, z * s);
    }
    Vector3 operator/(T s) const
    {
        return Vector3(x / s, y / s, z / s);
    }
    bool operator==(const Vector3& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }
    Vector3 Lerp(float t, const Vector3& v) const
    {
        return Vector3(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t,
                       z * (1 - t) + v.z * t);
    }
    const T* Pointer() const
    {
        return &x;
    }
    template <typename P>
    P* Write(P* pData)
    {
        Vector3<T>* pVector = (Vector3<T>*) pData;
        *pVector++ = *this;
        return (P*) pVector;
    }
    T x;
    T y;
    T z;
};

template <typename T>
struct Vector4 {
    Vector4() {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
    T Dot(const Vector4& v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }
    Vector4 Lerp(float t, const Vector4& v) const
    {
        return Vector4(x * (1 - t) + v.x * t,
                       y * (1 - t) + v.y * t,
                       z * (1 - t) + v.z * t,
                       w * (1 - t) + v.w * t);
    }
    const T* Pointer() const
    {
        return &x;
    }
    T x;
    T y;
    T z;
    T w;
};

typedef Vector2<bool> bvec2;

typedef Vector2<int> ivec2;
typedef Vector3<int> ivec3;
typedef Vector4<int> ivec4;

typedef Vector2<float> vec2;
typedef Vector3<float> vec3;
typedef Vector4<float> vec4;

std::ostream& operator<<(std::ostream& os, const vec2& v);
#endif
