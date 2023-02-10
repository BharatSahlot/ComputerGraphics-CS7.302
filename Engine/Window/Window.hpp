#ifndef WINDOW_H
#define WINDOW_H

#include "Engine/Camera.hpp"
#include "Engine/World.hpp"
#include "GLFW/glfw3.h"

#include <functional>
#include <map>
#include <memory>

// so bad, not so bad
extern float windowFade;

class Window
{
    public:
        std::shared_ptr<Camera> camera;
        using RenderCallback = std::function<bool(World* world)>;

        static Window* Create(int width, int height, const char* title);

        int Init();

        // changes the active camera
        void SetCamera(std::shared_ptr<Camera> camera);

        void MakeCurrent();
        
        void Render(World* world);
        
        void SetRenderCallback(RenderCallback callback);
        
        bool ShouldClose();
        
        float Aspect() const;
        int Width() const { return width; }
        int Height() const { return height; }

        glm::mat4 ViewProj() const { return camera->ViewProj(); }
        glm::vec3 ViewportPointToWorld(glm::vec3 point) const;
        glm::vec3 WorldToViewportPoint(glm::vec2 point) const;

        int GetKey(int key) const { return glfwGetKey(this->glfwWindow, key); }
        int GetKeyDown(int key) const { return GetKey(key) == GLFW_PRESS; }

        glm::vec2 GetCursorDelta() const { return cursorDelta; }


        GLFWwindow* GetGLFWwindow() const { return glfwWindow; }

        ~Window();

    private:
        int width;
        int height;

        glm::vec2 cursorPos;
        glm::vec2 cursorDelta;
        
        GLFWwindow* glfwWindow;

        RenderCallback cb;

        static void FramebufferSizeCallback(GLFWwindow* win, int width, int height);
        static void CursorMoveCallback(GLFWwindow* win, double x, double y);
};

#endif
