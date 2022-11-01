#include "stdafx.h"
#include "mcpp/aabb.h"
#include "mcpp/world/entity/entity.h"

mcpp::Entity::Entity(World* _world) :
    onGround(false), removed(false),
    yaw(0.0f), pitch(0.0f),
    bbWidth(0.6f), bbHeight(1.8f),
    world(_world),
    bb(nullptr),
    prevPosition(Vector3f()),
    position(Vector3f()),
    velocity(Vector3f()) {}

mcpp::Entity::~Entity() {
    delete bb;
    bb = nullptr;
}

void mcpp::Entity::moveRelative(float xa, float za, float speed) {
    float dist = xa * xa + za * za;
    if (dist >= 0.01) {
        dist = speed / sqrt(dist);
        xa *= dist;
        za *= dist;
        float sin = ::sin(-yaw);
        float cos = ::cos(-yaw);
        velocity.x += xa * cos - za * sin;
        velocity.z += za * cos + xa * sin;
    }
}

void mcpp::Entity::tick() {
    prevPosition.set(position);
}
