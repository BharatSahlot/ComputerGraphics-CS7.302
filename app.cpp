#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Window/Window.hpp"
#include "GLFW/glfw3.h"
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

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;

    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera);
    camera->view = glm::mat4(1.0f);

    Window* window = Window::Create(800, 600, "pls");
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    std::shared_ptr<Material> baseMat(mat);

    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, -10.f);
    camera->view = glm::lookAt(cameraPos,
                                  glm::vec3(0, 0, 0),
                                  glm::vec3(0, 1, 0));

    std::vector<float> vertices({
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f
    });

    std::vector<int> indices({
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    });

    Mesh* mesh = new Mesh(vertices, indices, false);

    std::shared_ptr<Mesh> baseMesh(mesh);

    Object o1(baseMesh, baseMat);
    Object o2(baseMesh, baseMat);

    o2.transform->SetParent(o1.transform);

    o1.transform->SetWorldPosition(glm::vec3(5, 0, 0));
    o2.transform->SetWorldPosition(glm::vec3(-5, 0, 0));

    o1.transform->SetLocalRotation(glm::vec3(0, 0, glm::radians(20.f)));


    Timer timer;
    timer.Start();
    window->SetRenderCallback([&](const Window& window) -> bool {
        float delta = timer.Tick();
        
        o1.material->SetVec3("col", glm::vec3(1, 0, 0));
        o1.Render(camera->view, camera->Proj());

        o2.material->SetVec3("col", glm::vec3(0, 1, 0));
        o2.Render(camera->view, camera->Proj());

        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
