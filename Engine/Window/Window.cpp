#include "Window.hpp"

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "GLFW/glfw3.h"
#include "Globals.hpp"
#include "glm/fwd.hpp"
#include <glad/glad.h>

#include <iostream>

float windowFade;

static std::map<GLFWwindow*, Window*> glfwToWindow;

Window* Window::Create(int width, int height, const char* title)
{
    Window* window = new Window;

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    window->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window->glfwWindow == NULL)
    {
        std::cerr << "Window error" << std::endl;
        delete(window);
        return nullptr;
    }

    window->width = width;
    window->height = height;

    glfwToWindow[window->glfwWindow] = window;

    glfwMakeContextCurrent(window->glfwWindow);
    glfwSetFramebufferSizeCallback(window->glfwWindow, Window::FramebufferSizeCallback);
    glfwSetCursorPosCallback(window->glfwWindow, Window::CursorMoveCallback);
    glfwSwapInterval(1);
    return window;
}

int Window::Init()
{
    return 0;
}

void Window::SetCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
    this->camera->SetPerspective(60.f, (float)this->width / this->height);
}

void Window::SetRenderCallback(RenderCallback callback)
{
    this->cb = callback;
}

Window::~Window()
{
    glfwDestroyWindow(this->glfwWindow);
}

void Window::MakeCurrent()
{
    glfwMakeContextCurrent(this->glfwWindow);
}

void Window::Render(World* world)
{
    if(!this->cb) return;

    glViewport(0, 0, this->width, this->height);

    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // glEnable(GL_LINE_SMOOTH);
    // glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(this->glfwWindow))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(this->cb(world))
        {
            break;
        }

        cursorDelta = glm::vec2(0);
        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}

float Window::Aspect() const
{
    return (float)this->width / this->height;
}

glm::vec3 Window::ViewportPointToWorld(glm::vec3 point) const
{
    glm::mat4 viewInv = glm::inverse(camera->ViewProj());
    return viewInv * glm::vec4(point, 1);
}

glm::vec3 Window::WorldToViewportPoint(glm::vec2 point) const
{
    return camera->ViewProj() * glm::vec4(point, 0, 1);
}

void Window::FramebufferSizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = glfwToWindow[win];
    window->height = height;
    window->width = width;
    if(window->camera)
    {
        window->camera->SetPerspective(60.0f, window->Aspect());
        // window->camera->SetOrthographic(width, height);
    }

    glViewport(0, 0, width, height);

    window->Init();
}

void Window::CursorMoveCallback(GLFWwindow* win, double x, double y)
{
    glm::vec2 pos(x, y);

    Window* window = glfwToWindow[win];

    glm::vec2 delta = window->cursorPos - pos;
    // delta.x /= window->width;
    // delta.y /= window->height;
    window->cursorDelta = delta;
    glfwSetCursorPos(win, (float)window->width / 2, (float)window->height / 2);

    window->cursorPos = pos;
}
