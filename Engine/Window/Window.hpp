#ifndef WINDOW_H
#define WINDOW_H

#include "Engine/Camera.hpp"
#include "GLFW/glfw3.h"

#include <functional>
#include <map>
#include <memory>

// so bad, not so bad
extern float windowFade;

class Window
{
    public:
        using RenderCallback = std::function<bool(const Window& window)>;

        static Window* Create(int width, int height, const char* title);

        int Init();

        // changes the active camera
        void SetCamera(std::shared_ptr<Camera> camera);

        void MakeCurrent();
        
        void Render();
        
        void SetRenderCallback(RenderCallback callback);
        
        bool ShouldClose();
        
        float Aspect() const;
        int Width() const { return width; }
        int Height() const { return height; }

        glm::mat4 ViewProj() const { return camera->ViewProj(); }
        glm::vec3 ViewportPointToWorld(glm::vec3 point) const;
        glm::vec3 WorldToViewportPoint(glm::vec2 point) const;

        int GetKey(int key) const { return glfwGetKey(this->glfwWindow, key); }
        ~Window();

    private:
        std::shared_ptr<Camera> camera;

        int width;
        int height;
        
        unsigned int framebuffer;
        unsigned int fbColBuffers[2];
        unsigned int rbo;

        unsigned int ppFbos[2];
        unsigned int ppBufs[2];

        GLFWwindow* glfwWindow;

        RenderCallback cb;

        static void FramebufferSizeCallback(GLFWwindow* win, int width, int height);
};

#endif
