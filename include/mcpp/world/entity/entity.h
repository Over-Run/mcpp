#pragma once
#include "mcpp/math/vector.h"

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

        virtual void remove();

        void turn(float yaw, float pitch);
        void move(float x, float y, float z);
        void moveRelative(float x, float z, float speed);

        virtual void tick();
    protected:
        void resetPos();
        void setPos(float x, float y, float z);
        void setSize(float w, float h);
    };
}
