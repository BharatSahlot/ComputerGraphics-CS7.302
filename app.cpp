#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Resource/ResourceManager.hpp"
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

#include <assimp/Importer.hpp>

#include "Globals.hpp"

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;
    srand(time(0));
    Random::Init();

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::mat4(1.0f);

    std::shared_ptr<Window> window(Window::Create(800, 600, "Racing game"));
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    window->Init();

    World* world = new World(window);

    world->GetResourceManager().AddInResourceQueue<Material>("baseMat", MaterialData {
        "Shaders/base.vs", "Shaders/base.fs"
    });

    world->GetResourceManager().AddInResourceQueue<Material>("textMat", MaterialData {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    world->GetResourceManager().AddInResourceQueue<Texture>("texture", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture1", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture2", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture3", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture4", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture5", std::string("Player_Run.png"));
    world->GetResourceManager().AddInResourceQueue<Texture>("texture6", std::string("Player_Run.png"));

    world->GetResourceManager().StartLoading();

    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](World* world) -> bool {
        float delta = timer.Tick();
        
        std::cout << world->GetResourceManager().GetLoadStatus() << std::endl;

        if(world->GetResourceManager().HasLoadingFinished())
        {
            world->GetResourceManager().Load();
            return true;
        }

        return false;
    });

    window->Render(world);

    delete world;

    EngineClean();
    return 0;
}
