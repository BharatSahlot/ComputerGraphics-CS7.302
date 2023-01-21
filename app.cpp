#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
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
    camera->viewMat = glm::mat4(1.0f);

    Window* window = Window::Create(800, 600, "pls");
    window->SetCamera(camera);
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    std::shared_ptr<Material> baseMat(mat);

    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, -3.f);
    camera->viewMat = glm::lookAt(cameraPos,
                                  glm::vec3(0, 0, 0),
                                  glm::vec3(0, 1, 0));

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
