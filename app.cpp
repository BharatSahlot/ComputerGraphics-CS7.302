#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "GLFW/glfw3.h"
#include "Game/Levels/Levels.hpp"
#include "Game/Player/Player.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

#include "Globals.hpp"

std::shared_ptr<Mesh> Plane;
std::shared_ptr<Material> BasicMat;
std::shared_ptr<Material> BasicTexMat;

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;
    srand(time(0));

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::mat4(1.0f);

    Window* window = Window::Create(800, 600, "pls");
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);
    Shader* texFrag = Shader::MakeShader("Shaders/tex.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    BasicMat = std::shared_ptr<Material>(mat);

    BasicTexMat = std::shared_ptr<Material>(
            Material::MakeMaterial(baseVert, texFrag));

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

    Mesh* mesh = new Mesh(vertices, indices, false);

    Plane = std::shared_ptr<Mesh>(mesh);

    window->Init();

    Level1 level1(camera);
    level1.Load();

    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](const Window& window) -> bool {
        float delta = timer.Tick();
        if(!level1.started)
        {
            level1.started = true;
            level1.Start();
        }
        
        level1.Tick(window, delta);
        level1.Render(camera->view, camera->Proj());
        return false;
    });

    window->Render();

    level1.Unload();

    EngineClean();
    return 0;
}
