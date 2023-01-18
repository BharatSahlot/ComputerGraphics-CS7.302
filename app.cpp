#include "Engine/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <iostream>
#include <vector>

// generates prism of radius 1 and length 2
Mesh GeneratePrism(int sides, std::shared_ptr<Material> mat)
{
    std::vector<float> vertices;
    std::vector<int> indices;

    float angle = 90.0f; // to keep one top vertex at the center
    const float delta = 360.0f / sides;

    for(int i = 0; i < sides; i++, angle += delta)
    {
        float x = glm::cos(glm::radians(angle));
        float y = glm::sin(glm::radians(angle));

        vertices.insert(vertices.end(), {x, y, 1.0f, 0.f, 0.f, 1.f});

        indices.push_back(i);
        indices.push_back((i + 1) % sides);
        indices.push_back(sides);
    }
    vertices.insert(vertices.end(), {0.0f, 0.0f, 1.0f, 0.f, 0.f, 1.f});

    angle = 90.0f;
    for(int i = 0; i < sides; i++, angle += delta)
    {
        float x = glm::cos(glm::radians(angle));
        float y = glm::sin(glm::radians(angle));

        vertices.insert(vertices.end(), {x, y, -1.0f, 0.f, 0.f, -1.f});

        indices.push_back(sides + 1 + i);
        indices.push_back(sides + 1 + ((i + 1) % sides));
        indices.push_back(sides + 1 + sides);
    }
    vertices.insert(vertices.end(), {0.0f, 0.0f, -1.0f, 0.f, 0.f, -1.f});

    angle = 90.0f;
    for(int i = 0; i < sides; i++, angle += delta)
    {
        float x1 = glm::cos(glm::radians(angle));
        float y1 = glm::sin(glm::radians(angle));

        float x2 = glm::cos(glm::radians(angle + delta));
        float y2 = glm::sin(glm::radians(angle + delta));

        glm::vec3 normal = glm::cross(glm::vec3(x1, y1, 0.0f) - glm::vec3(x2, y2, 0.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f));

        int v = (sides + 1) * 2 + i * 4;
        vertices.insert(vertices.end(), {x1, y1, 1.0f , normal.x, normal.y, normal.z});
        vertices.insert(vertices.end(), {x1, y1, -1.0f, normal.x, normal.y, normal.z});
        vertices.insert(vertices.end(), {x2, y2, 1.0f , normal.x, normal.y, normal.z});
        vertices.insert(vertices.end(), {x2, y2, -1.0f, normal.x, normal.y, normal.z});

        indices.push_back(v);
        indices.push_back(v + 1);
        indices.push_back(v + 3);

        indices.push_back(v);
        indices.push_back(v + 3);
        indices.push_back(v + 2);
    }

    return Mesh(vertices, indices, mat);
}

int main(int argc, const char** argv)
{
    if(EngineInit() == -1) return -1;

    if(argc != 2)
    {
        std::cout << "Wrong number of arguements provided. Provide exactly 1 arguement." << std::endl;
        EngineClean();
        return -1;
    }

    int sides = std::atoi(argv[1]);

    Window* window = Window::Create(800, 600, "pls");
    window->MakeCurrent();

    EngineInitGLAD();

    Shader* baseVert = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* baseFrag = Shader::MakeShader("Shaders/base.fs", GL_FRAGMENT_SHADER);

    Material* mat = Material::MakeMaterial(baseVert, baseFrag);
    std::shared_ptr<Material> baseMat(mat);

    Mesh mesh = GeneratePrism(sides, baseMat);

    mesh.mMat = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    window->camera.viewMat = glm::translate(window->camera.viewMat, glm::vec3(0.0f, 0.0f, -3.0f));

    window->SetRenderCallback([&](const Window& window) -> bool {

        mesh.Render(window.camera.ViewProj());

        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
