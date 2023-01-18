#include "Window.hpp"

#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include <glad/glad.h>

#include <iostream>

// TODO:: handle framebuffer size change

static std::map<GLFWwindow*, Window*> glfwToWindow;

Window* Window::Create(int width, int height, const char* title)
{
    Window* window = new Window;

    window->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window->glfwWindow == NULL)
    {
        std::cerr << "Window error" << std::endl;
        delete(window);
        // TODO: log error
        return nullptr;
    }

    window->width = width;
    window->height = height;
    window->camera.viewMat = glm::mat4(1.0f);
    window->camera.SetPerspective(60.f, (float)width / height);

    glfwToWindow[window->glfwWindow] = window;

    glfwSetFramebufferSizeCallback(window->glfwWindow, Window::FramebufferSizeCallback);
    return window;
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

void Window::Render()
{
    if(!this->cb) return;

    glViewport(0, 0, this->width, this->height);

    while(!glfwWindowShouldClose(this->glfwWindow))
    {
        glViewport(0, 0, this->width, this->height);
        if(this->cb(*this))
        {
            break;
        }

        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}

float Window::Aspect()
{
    return (float)this->width / this->height;
}

void Window::FramebufferSizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = glfwToWindow[win];
    window->height = height;
    window->width = width;
    window->camera.SetPerspective(60.0f, window->Aspect());
}
