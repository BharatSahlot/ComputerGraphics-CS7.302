#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"

#include <functional>

class Window
{
    public:
        using RenderCallback = std::function<bool(const Window& window)>;

        static Window* Create(int width, int height, const char* title);
        void MakeCurrent();
        void Render();
        void SetRenderCallback(RenderCallback callback);
        bool ShouldClose();
        ~Window();

    private:
        int width;
        int height;
        GLFWwindow* glfwWindow;

        RenderCallback cb;
};

#endif
