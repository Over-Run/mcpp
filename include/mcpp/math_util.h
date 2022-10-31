#pragma once

namespace mcpp {
    enum class direction : unsigned char {
        WEST, EAST, DOWN, UP, NORTH, SOUTH
    };

    namespace math {
        constexpr float PI_f = 3.1415927f;
        constexpr double PI_d = 3.141592653589793;
        constexpr float RAD90_f = PI_f * 0.5f;
        constexpr float RAD180_f = PI_f;
        constexpr float RAD360_f = PI_f * 2.0f;
        constexpr double RAD90_d = PI_d * 0.5;
        constexpr double RAD180_d = PI_d;
        constexpr double RAD360_d = PI_d * 2.0;

        template<typename T, typename T_delta>
        inline T lerp(T x0, T x1, T_delta delta) {
            return x0 + (x1 - x0) * (T)delta;
        }

        inline constexpr float toRadians(float deg) {
            constexpr float DEG_TO_RAD = PI_f / 180.0f;
            return deg * DEG_TO_RAD;
        }

        inline constexpr double toRadians(double deg) {
            constexpr double DEG_TO_RAD = PI_d / 180.0;
            return deg * DEG_TO_RAD;
        }
    }
}
