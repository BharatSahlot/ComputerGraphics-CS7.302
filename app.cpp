#include "Engine/Engine.hpp"
#include "Engine/Window/Window.hpp"

#include <glad/glad.h>

bool RenderCallback(const Window& window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return false;
}

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;

    Window* window = Window::Create(800, 600, "pls");
    window->SetRenderCallback(RenderCallback);
    window->MakeCurrent();

    EngineInitGLAD();

    window->Render();

    EngineClean();
    return 0;
}
