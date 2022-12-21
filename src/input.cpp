#include "stdafx.h"
#include "mcpp/input.h"

mcpp::Window* mcpp::window = nullptr;

mcpp::Window::Window(int width, int height, const char* title) :
    window(glfwCreateWindow(width, height, title, NULL, NULL)) {
}

mcpp::Window::~Window() {
    glfwDestroyWindow(window);
    window = NULL;
}

GLFWwindow* mcpp::Window::handle() const {
    return window;
}

void mcpp::Window::getFramebufferSize(int* width, int* height) const {
    glfwGetFramebufferSize(window, width, height);
}

void mcpp::Window::getSize(int* width, int* height) const {
    glfwGetWindowSize(window, width, height);
}

void mcpp::Window::setPos(int xpos, int ypos) const {
    glfwSetWindowPos(window, xpos, ypos);
}

void mcpp::Window::setInputMode(int mode, int value) const {
    glfwSetInputMode(window, mode, value);
}

int mcpp::Window::getKey(int key) const {
    return glfwGetKey(window, key);
}

void mcpp::Window::setKeyCallback(GLFWkeyfun callback) const {
    glfwSetKeyCallback(window, callback);
}

void mcpp::Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(window, callback);
}

void mcpp::Window::setCursorPosCallback(GLFWcursorposfun callback) const {
    glfwSetCursorPosCallback(window, callback);
}

void mcpp::Window::makeContextCurrent() const {
    glfwMakeContextCurrent(window);
}

void mcpp::Window::show() const {
    glfwShowWindow(window);
}

bool mcpp::Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void mcpp::Window::swapBuffers() const {
    glfwSwapBuffers(window);
}
