#pragma once
#include "mcpp/vector.h"

namespace mcpp {
    class AABBox;
    class World;

    class Entity {
    public:
        bool onGround, removed;
        float yaw, pitch;
        float bbWidth, bbHeight;
        World* world;
        AABBox* bb;
        Vector3f prevPosition, position, velocity;

        Entity(World* world);
        virtual ~Entity();

        void moveRelative(float xa, float za, float speed);
        virtual void tick();
    };
}
