#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Font.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/UI/Text.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Utils/Random.hpp"
#include "Engine/World.hpp"
#include "GLFW/glfw3.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

#include "Game/Game.hpp"
#include "Globals.hpp"
#include "glm/ext/matrix_transform.hpp"

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;
    Random::Init();

    std::shared_ptr<Window> window(Window::Create(800, 600, "Racing game"));
    window->MakeCurrent();

    EngineInitGLAD();
    window->Init();


    // block so it cleans resources before EngineClean()
    {
        Game game(window);

        Timer timer;
        timer.Start();
        window->SetRenderCallback([&]() -> bool {
            float delta = timer.Tick();
            game.Tick(delta);
            game.Render();
            return false;
        });

        window->Render();
    }

    // game.Clean();
    EngineClean();
    return 0;
}
