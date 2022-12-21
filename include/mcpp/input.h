#pragma once
#include "stdafx.h"

namespace mcpp {
    class Window {
    private:
        GLFWwindow* window;
    public:
        Window(int width, int height, const char* title);
        ~Window();
        GLFWwindow* handle() const;

        void getFramebufferSize(int* width, int* height) const;
        void getSize(int* width, int* height) const;
        void setPos(int xpos, int ypos) const;

        void setInputMode(int mode, int value) const;
        int getKey(int key) const;

        void setKeyCallback(GLFWkeyfun callback) const;
        void setFramebufferSizeCallback(GLFWframebuffersizefun callback) const;
        void setCursorPosCallback(GLFWcursorposfun callback) const;

        void makeContextCurrent() const;
        void show() const;
        bool shouldClose() const;
        void swapBuffers() const;
    };

    extern Window* window;
}
