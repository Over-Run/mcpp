#pragma once
#include "math_util.h"

namespace mcpp {
    template<typename T>
    struct Vector3 {
    public:
        T x, y, z;

        Vector3(T _x, T _y, T _z) :
            x(_x), y(_y), z(_z) {}

        Vector3() :
            Vector3(0, 0, 0) {}

        Vector3& set(const Vector3& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        Vector3& set(T _x, T _y, T _z) {
            x = _x;
            y = _y;
            z = _z;
            return *this;
        }

        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3& operator*=(const Vector3& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        template<typename T_delta>
        Vector3& lerp(const Vector3& other, T_delta delta, Vector3& dest) const {
            dest.x = (T)math::lerp(x, other.x, delta);
            dest.y = (T)math::lerp(y, other.y, delta);
            dest.z = (T)math::lerp(z, other.z, delta);
            return dest;
        }

        template<typename T_delta>
        Vector3& lerp(const Vector3& other, T_delta delta) {
            x = (T)math::lerp(x, other.x, delta);
            y = (T)math::lerp(y, other.y, delta);
            z = (T)math::lerp(z, other.z, delta);
            return *this;
        }

        T distanceSqr(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        double distance(const Vector3& other) const {
            return sqrt((double)(x * other.x + y * other.y + z * other.z));
        }
    };

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int>;


    template<typename T>
    struct Vector4 {
    public:
        T x, y, z, w;

        Vector4(T _x, T _y, T _z, T _w) :
            x(_x), y(_y), z(_z), w(_w) {}

        Vector4() :
            Vector4(0, 0, 0, 0) {}

        Vector4& set(const Vector4& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        Vector4& set(T _x, T _y, T _z, T _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
            return *this;
        }

        Vector4& operator+=(const Vector4& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Vector4& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vector4& operator*=(const Vector4& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        }

        template<typename T_delta>
        Vector4& lerp(const Vector4& other, T_delta delta, Vector4& dest) const {
            dest.x = (T)math::lerp(x, other.x, delta);
            dest.y = (T)math::lerp(y, other.y, delta);
            dest.z = (T)math::lerp(z, other.z, delta);
            dest.w = (T)math::lerp(w, other.w, delta);
            return dest;
        }

        template<typename T_delta>
        Vector4& lerp(const Vector4& other, T_delta delta) {
            x = (T)math::lerp(x, other.x, delta);
            y = (T)math::lerp(y, other.y, delta);
            z = (T)math::lerp(z, other.z, delta);
            w = (T)math::lerp(w, other.w, delta);
            return *this;
        }

        T distanceSqr(const Vector4& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        double distance(const Vector4& other) const {
            return sqrt((double)(x * other.x + y * other.y + z * other.z + w * other.w));
        }
    };

    using Vector4f = Vector4<float>;
    using Vector4i = Vector4<int>;
}
