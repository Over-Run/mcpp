#include "stdafx.h"
#include "mcpp/player.h"

mcpp::player::player() :
    onGround(false), removed(false),
    yaw(0.0f), pitch(0.0f),
    bbWidth(0.6f), bbHeight(1.8f),
    prevPosition(vector3f()),
    position(vector3f()),
    velocity(vector3f()) {}

void mcpp::player::moveRelative(float xa, float za, float speed) {
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

void mcpp::player::tick() {
    prevPosition.set(position);
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        --yo;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        ++yo;
    }
    if (glfwGetKey(window, GLFW_KEY_W))
    {
        --zo;
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        ++zo;
    }
    moveRelative(xo, zo, onGround ? 0.1f : 0.02f);
    // yd -= 0.08
    // todo replace this below with move(velocity)
    position += velocity;
    // todo remove this
    position.y += yo * 0.15f;
    velocity.x *= 0.91f;
    velocity.y *= 0.98f;
    velocity.z *= 0.91f;
    if (onGround) {
        velocity.x *= 0.7f;
        velocity.z *= 0.7f;
    }
}
