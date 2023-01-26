#include "Engine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int EngineInit()
{
    glfwInit();

    // window hints, but for now same for every window
    // should move to Window::Create later
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    return 0;
}

// stupid why glad needs context set before init
int EngineInitGLAD()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Glad error" << std::endl;
        return -1;
    }
    return 0;
}

int EngineClean()
{
    glfwTerminate();
    return 0;
}
