#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;

    Window* window = Window::Create(800, 600, "pls");
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    std::shared_ptr<Material> baseMat(mat);

    Mesh mesh(std::vector<float>({
                0.5f,   0.5f,   0.0f,
                0.5f,  -0.5f,   0.0f,
                -0.5f, -0.5f,   0.0f,
                -0.5f,  0.5f,   0.0f
            }),
            std::vector<int>({
                0, 1, 3,
                1, 2, 3
            }),
            baseMat);

    mesh.mMat = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    window->camera.viewMat = glm::translate(window->camera.viewMat, glm::vec3(0.0f, 0.0f, -3.0f));

    window->SetRenderCallback([&](const Window& window) -> bool {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mesh.Render(window.camera.ViewProj());

        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
