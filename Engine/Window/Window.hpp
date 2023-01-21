#ifndef WINDOW_H
#define WINDOW_H

#include "Engine/Camera.hpp"
#include "GLFW/glfw3.h"

#include <functional>
#include <map>
#include <memory>

class Window
{
    public:
        using RenderCallback = std::function<bool(const Window& window)>;

        static Window* Create(int width, int height, const char* title);

        // changes the active camera
        void SetCamera(std::shared_ptr<Camera> camera);

        void MakeCurrent();
        
        void Render();
        
        void SetRenderCallback(RenderCallback callback);
        
        bool ShouldClose();
        
        float Aspect() const;

        int GetKey(int key) const { return glfwGetKey(this->glfwWindow, key); }
        ~Window();

    private:
        std::shared_ptr<Camera> camera;

        int width;
        int height;
        GLFWwindow* glfwWindow;

        RenderCallback cb;

        static void FramebufferSizeCallback(GLFWwindow* win, int width, int height);
};

#endif
