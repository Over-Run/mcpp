#include "stdafx.h"
#include "mcpp/input.h"
#include "mcpp/world/entity/player.h"

mcpp::PlayerEntity::PlayerEntity(World* world) : Entity(world) {
    eyeHeight = 1.62f;
}

void mcpp::PlayerEntity::tick() {
    Entity::tick();
    float xo = 0.0f, yo = 0.0f, zo = 0.0f;
    if (mcpp::window->getKey(GLFW_KEY_A))
    {
        --xo;
    }
    if (mcpp::window->getKey(GLFW_KEY_D))
    {
        ++xo;
    }
    if (mcpp::window->getKey(GLFW_KEY_W))
    {
        --zo;
    }
    if (mcpp::window->getKey(GLFW_KEY_S))
    {
        ++zo;
    }
    if (mcpp::window->getKey(GLFW_KEY_SPACE))
    {
        velocity.y = 0.5f;
    }
    moveRelative(xo, zo, onGround ? 0.1f : 0.02f);
    velocity.y -= 0.08f;
    move(velocity);
    velocity.x *= 0.91f;
    velocity.y *= 0.98f;
    velocity.z *= 0.91f;
    if (onGround) {
        velocity.x *= 0.7f;
        velocity.z *= 0.7f;
    }
}
