#pragma once

namespace mcpp {
    enum class Direction : unsigned char {
        WEST, EAST, DOWN, UP, NORTH, SOUTH
    };

    inline constexpr int dirOffsetX(Direction dir) {
        switch (dir)
        {
        case mcpp::Direction::WEST:
            return -1;
        case mcpp::Direction::EAST:
            return 1;
        case mcpp::Direction::DOWN:
        case mcpp::Direction::UP:
        case mcpp::Direction::NORTH:
        case mcpp::Direction::SOUTH:
        default:
            return 0;
        }
    }

    inline constexpr int dirOffsetY(Direction dir) {
        switch (dir)
        {
        case mcpp::Direction::DOWN:
            return -1;
        case mcpp::Direction::UP:
            return 1;
        case mcpp::Direction::WEST:
        case mcpp::Direction::EAST:
        case mcpp::Direction::NORTH:
        case mcpp::Direction::SOUTH:
        default:
            return 0;
        }
    }

    inline constexpr int dirOffsetZ(Direction dir) {
        switch (dir)
        {
        case mcpp::Direction::NORTH:
            return -1;
        case mcpp::Direction::SOUTH:
            return 1;
        case mcpp::Direction::WEST:
        case mcpp::Direction::EAST:
        case mcpp::Direction::DOWN:
        case mcpp::Direction::UP:
        default:
            return 0;
        }
    }

    inline constexpr Direction dirOpposite(Direction dir) {
        switch (dir)
        {
        case mcpp::Direction::WEST:
            return Direction::EAST;
        case mcpp::Direction::EAST:
            return Direction::WEST;
        case mcpp::Direction::DOWN:
            return Direction::UP;
        case mcpp::Direction::UP:
            return Direction::DOWN;
        case mcpp::Direction::NORTH:
            return Direction::SOUTH;
        case mcpp::Direction::SOUTH:
        default:
            return Direction::NORTH;
        }
    }

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
        inline T_delta lerp(T x0, T x1, T_delta delta) {
            return (T_delta)((T_delta)x0 + (x1 - x0) * delta);
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
