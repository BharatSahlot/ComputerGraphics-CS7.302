#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Utils/Random.hpp"
#include "GLFW/glfw3.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

#include "Globals.hpp"


int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;
    srand(time(0));
    Random::Init();

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::mat4(1.0f);

    Window* window = Window::Create(800, 600, "pls");
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](const Window& window) -> bool {
        float delta = timer.Tick();
        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
