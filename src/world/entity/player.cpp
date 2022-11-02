#include "stdafx.h"
#include "mcpp/world/entity/player.h"

mcpp::Player::Player(World* world) : Entity(world) {
    eyeHeight = 1.62f;
}

void mcpp::Player::tick() {
    Entity::tick();
    float xo = 0.0f, yo = 0.0f, zo = 0.0f;
    // TODO: Replace this with keyboard::isKeyDown
    GLFWwindow* window = glfwGetCurrentContext();
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        --xo;
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        ++xo;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        --zo;
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        ++zo;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE))
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
