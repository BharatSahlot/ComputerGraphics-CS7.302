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
        "Car/sportste.fbx"
    });

    // auto roadsModel = world->GetResourceManager().AddInResourceQueue("roads", ResourceLoadData<Model> {
    //     "Car/Low Poly Road Pack.obj"
    // });

    world->GetResourceManager().StartLoading();

    // auto car = world->Instantiate<Object>("Car", carModel);
    std::shared_ptr<Object> car;
    const Object* wheelFL = nullptr;
    const Object* wheelFR = nullptr;

    auto camera = world->Instantiate<Camera>("Camera");
    window->SetCamera(camera);
    camera->Start();

    float angle = 0.f;
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
            car = world->Instantiate<Object>("Car", carModel);
            wheelFL = world->GetObjectByName<Object>("WheelFL");
            wheelFR = world->GetObjectByName<Object>("WheelFR");

            // world->Instantiate<Object>("Roads", roadsModel);
        }

        if(!loaded)
        {
            std::cout << world->GetResourceManager().GetLoadStatus() << std::endl;
            return false;
        }

        if(window->GetKeyDown(GLFW_KEY_LEFT)) angle += delta * 70.f;
        if(window->GetKeyDown(GLFW_KEY_RIGHT)) angle -= delta * 70.f;

        angle = glm::clamp(angle, -30.f, 30.f);
        wheelFL->transform->SetLocalRotation(glm::vec3(0, 0, glm::radians(angle)));
        wheelFR->transform->SetLocalRotation(glm::vec3(0, 0, glm::radians(angle)));
        car->transform->SetLocalRotation(glm::vec3(0, glm::radians(angle), 0));
        world->Tick(delta);
        world->Render();

        return false;
    });

    window->Render(world);

    delete world;

    EngineClean();
    return 0;
}
