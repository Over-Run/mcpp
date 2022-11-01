#include "stdafx.h"
#include "mcpp/math/aabb.h"
#include "mcpp/world/entity/entity.h"
#include "mcpp/world/world.h"

mcpp::Entity::Entity(World* _world) :
    onGround(false), removed(false),
    yaw(0.0f), pitch(0.0f),
    bbWidth(0.6f), bbHeight(1.8f),
    world(_world),
    bb(nullptr),
    prevPosition(Vector3f()),
    position(Vector3f()),
    velocity(Vector3f()) {
    resetPos();
}

mcpp::Entity::~Entity() {
    delete bb;
    bb = nullptr;
}

void mcpp::Entity::remove() {
    removed = true;
}

void mcpp::Entity::turn(float _yaw, float _pitch) {
    yaw += _yaw;
    if (yaw > mcpp::math::RAD180_f)
    {
        yaw -= mcpp::math::RAD360_f;
    }
    if (yaw < -mcpp::math::RAD180_f)
    {
        yaw += mcpp::math::RAD360_f;
    }
    pitch += _pitch;
    if (pitch > mcpp::math::RAD90_f)
    {
        pitch = mcpp::math::RAD90_f;
    }
    else if (pitch < -mcpp::math::RAD90_f)
    {
        pitch = -mcpp::math::RAD90_f;
    }
}

void mcpp::Entity::move(float x, float y, float z) {
    float xaOrg = x;
    float yaOrg = y;
    float zaOrg = z;
    std::vector<AABBox*> boxes = std::vector<AABBox*>();
    AABBox box = AABBox();
    world->getCubes(bb->expand(x, y, z, box), boxes);

    for (auto& _box : boxes) {
        y = _box->clipYCollide(*bb, y);
    }
    bb->move(0.0f, y, 0.0f);

    for (auto& _box : boxes) {
        x = _box->clipXCollide(*bb, x);
    }
    bb->move(x, 0.0f, 0.0f);

    for (auto& _box : boxes) {
        z = _box->clipZCollide(*bb, z);
    }
    bb->move(0.0f, 0.0f, z);

    onGround = yaOrg != y && yaOrg < 0.0f;

    if (xaOrg != x) { velocity.x = 0.0f; }
    if (yaOrg != y) { velocity.y = 0.0f; }
    if (zaOrg != z) { velocity.z = 0.0f; }

    position.x = (bb->minX + bb->maxX) * 0.5f;
    position.y = bb->minY;
    position.z = (bb->minZ + bb->maxZ) * 0.5f;
}

void mcpp::Entity::moveRelative(float x, float z, float speed) {
    float dist = x * x + z * z;
    if (dist >= 0.01) {
        dist = speed / sqrt(dist);
        x *= dist;
        z *= dist;
        float sin = ::sin(-yaw);
        float cos = ::cos(-yaw);
        velocity.x += x * cos - z * sin;
        velocity.z += z * cos + x * sin;
    }
}

void mcpp::Entity::tick() {
    prevPosition.set(position);
}
