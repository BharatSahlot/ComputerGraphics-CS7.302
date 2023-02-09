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
    srand(time(0));
    Random::Init();

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::lookAt(glm::vec3(0, 2.f, -3.f),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));

    std::shared_ptr<Window> window(Window::Create(800, 600, "Racing game"));
    window->SetCamera(camera);
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

    auto baseTex = world->GetResourceManager().AddInResourceQueue<Texture>("texture1", ResourceLoadData<Texture> {
        "bg.png", GL_NEAREST
    });

    std::vector<float> vertices({
            0.5f,  0.5f, 0.0f, 1.f, 1.f,  // top right
            0.5f, -0.5f, 0.0f, 1.f, 0.f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.f, 0.f, // bottom left
            -0.5f,  0.5f, 0.0f, 0.f, 1.f
    });

    std::vector<int> indices({
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    });

    std::shared_ptr<Mesh> plane(new Mesh(vertices, indices, false));
    Object object("o", plane, baseMat);
    object.UseTexture(baseTex);

    world->GetResourceManager().StartLoading();

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

        object.Render(camera->view, camera->Proj());

        return false;
    });

    window->Render(world);

    delete world;

    EngineClean();
    return 0;
}
