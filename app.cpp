#include "Engine/Engine.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Window/Window.hpp"

#include <glad/glad.h>
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

    window->SetRenderCallback([&](const Window& _) -> bool {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mesh.Render();

        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
