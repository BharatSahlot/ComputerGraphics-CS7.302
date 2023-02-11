#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
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

    World* world = new World(window);

    auto baseMat = world->GetResourceManager().AddInResourceQueue<Material>("baseMat", ResourceLoadData<Material> {
        "Shaders/base.vs", "Shaders/base.fs"
    });

    world->GetResourceManager().AddInResourceQueue<Material>("textMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    auto carModel = world->GetResourceManager().AddInResourceQueue("mesh", ResourceLoadData<Model> {
        // "Car/p6.obj"
        "Car/sportste.obj"
    });

    world->GetResourceManager().StartLoading();

    world->Instantiate<Object>("Car", carModel);

    auto camera = world->Instantiate<Camera>("Camera");
    window->SetCamera(camera);
    camera->Start();

    windowFade = 1.f;
    bool loaded = false;
    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](World* world) -> bool {
        float delta = timer.Tick();
        
        if(!loaded && world->GetResourceManager().HasLoadingFinished())
        {
            world->GetResourceManager().Load();
            loaded = true;
        }

        if(!loaded)
        {
            std::cout << world->GetResourceManager().GetLoadStatus() << std::endl;
            return false;
        }

        world->Tick(delta);
        world->Render();

        return false;
    });

    window->Render(world);

    delete world;

    EngineClean();
    return 0;
}
