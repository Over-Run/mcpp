#pragma once
#include "vector.h"

namespace mcpp {
    class player {
    public:
        bool onGround, removed;
        float yaw, pitch;
        float bbWidth, bbHeight;
        vector3f prevPosition, position, velocity;

        player();

        void moveRelative(float xa, float za, float speed);
        void tick();
    };
}
