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
        Vector3& lerp(const Vector3& other, T_delta delta, Vector3& dest) {
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

        T distanceSqr(const Vector3& other) {
            return x * other.x + y * other.y + z * other.z;
        }

        double distance(const Vector3& other) {
            return sqrt((double)(x * other.x + y * other.y + z * other.z));
        }
    };

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int>;
}
