#pragma once
#include "math_util.h"

namespace mcpp {
    template<typename T>
    struct vector3 {
    public:
        T x, y, z;

        vector3(T _x, T _y, T _z) :
            x(_x), y(_y), z(_z) {}

        vector3() :
            vector3(0, 0, 0) {}

        vector3& set(const vector3& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        vector3& operator+=(const vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        vector3& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        vector3& operator*=(const vector3& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        template<typename T_delta>
        vector3& lerp(const vector3& other, T_delta delta, vector3& dest) {
            dest.x = math::lerp(x, other.x, delta);
            dest.y = math::lerp(y, other.y, delta);
            dest.z = math::lerp(z, other.z, delta);
            return dest;
        }

        template<typename T_delta>
        vector3& lerp(const vector3& other, T_delta delta) {
            x = math::lerp(x, other.x, delta);
            y = math::lerp(y, other.y, delta);
            z = math::lerp(z, other.z, delta);
            return *this;
        }
    };

    using vector3f = vector3<float>;
}
