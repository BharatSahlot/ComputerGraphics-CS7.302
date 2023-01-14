#include "Window.hpp"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include <iostream>

// TODO:: handle framebuffer size change

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
        if(this->cb(*this))
        {
            break;
        }

        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}
