#include "Window.hpp"

#include "glm/fwd.hpp"
#include <glad/glad.h>

#include <iostream>

static std::map<GLFWwindow*, Window*> glfwToWindow;

Window* Window::Create(int width, int height, const char* title)
{
    Window* window = new Window;

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

    glfwSetFramebufferSizeCallback(window->glfwWindow, Window::FramebufferSizeCallback);
    return window;
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

void Window::Render()
{
    if(!this->cb) return;

    glViewport(0, 0, this->width, this->height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(this->glfwWindow))
    {
        glViewport(0, 0, this->width, this->height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(this->cb(*this))
        {
            break;
        }

        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}

float Window::Aspect() const
{
    return (float)this->width / this->height;
}

void Window::FramebufferSizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = glfwToWindow[win];
    window->height = height;
    window->width = width;
    if(window->camera)
    {
        // window->camera->SetPerspective(60.0f, window->Aspect());
        window->camera->SetOrthographic(width, height);
    }
}
