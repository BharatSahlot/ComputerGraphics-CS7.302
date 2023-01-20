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

        indices.push_back(sides + 1 + sides);
        indices.push_back(sides + 1 + ((i + 1) % sides));
        indices.push_back(sides + 1 + i);
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

Mesh GeneratePyramid(int sides, std::shared_ptr<Material> mat)
{
    std::vector<float> vertices;
    std::vector<int> indices;

    float angle = 90.0f; // to keep one top vertex at the center
    const float delta = 360.0f / sides;

    angle = 90.0f;
    for(int i = 0; i < sides; i++, angle += delta)
    {
        float x = glm::cos(glm::radians(angle));
        float y = glm::sin(glm::radians(angle));

        vertices.insert(vertices.end(), {x, y, -1.0f, 0.f, 0.f, -1.f});

        indices.push_back((i + 1) % sides);
        indices.push_back(i);
        indices.push_back(sides);
    }
    vertices.insert(vertices.end(), {0.0f, 0.0f, -1.0f, 0.f, 0.f, -1.f});

    angle = 90.0f;
    for(int i = 0; i < sides; i++, angle += delta)
    {
        float x1 = glm::cos(glm::radians(angle));
        float y1 = glm::sin(glm::radians(angle));

        float x2 = glm::cos(glm::radians(angle + delta));
        float y2 = glm::sin(glm::radians(angle + delta));

        glm::vec3 normal = glm::cross(glm::vec3(x1, y1, 1.0f) - glm::vec3(x2, y2, 1.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f));

        int v = sides + 1 + i * 3;
        vertices.insert(vertices.end(), {x1, y1, -1.0f, normal.x, normal.y, normal.z});
        vertices.insert(vertices.end(), {x2, y2, -1.0f, normal.x, normal.y, normal.z});
        vertices.insert(vertices.end(), {0.f, 0.f, 1.f, normal.x, normal.y, normal.z});

        indices.push_back(v);
        indices.push_back(v + 1);
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

    // window should instead keep a pointer to the current camera
    Mesh prism = GeneratePrism(sides, baseMat);
    Mesh pyramid = GeneratePyramid(sides, baseMat);

    Mesh* current = &prism;
    bool isRenderingPrism = true;

    prism.mMat = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, -3.f);
    camera->viewMat = glm::lookAt(cameraPos,
                                  glm::vec3(0, 0, 0),
                                  glm::vec3(0, 1, 0));

    glm::vec3 meshPos = glm::vec3(0, 0, 0);
    float meshRot = -35.0f;

    Timer timer, meshChangeTimer;
    timer.Start();
    meshChangeTimer.Start();
    window->SetRenderCallback([&](const Window& window) -> bool {

        float delta = timer.Tick();

        if(window.GetKey(GLFW_KEY_T) == GLFW_PRESS && meshChangeTimer.TimeSinceStart() >= 0.2f)
        {
            meshChangeTimer.Start();
            if(isRenderingPrism)
            {
                isRenderingPrism = false;
                current = &pyramid;
            } else
            {
                isRenderingPrism = true;
                current = &prism;
            }
        }

        glm::vec3 camMoveVec(0, 0, 0);
        if(window.GetKey(GLFW_KEY_W) == GLFW_PRESS) camMoveVec.z += 1;
        if(window.GetKey(GLFW_KEY_S) == GLFW_PRESS) camMoveVec.z -= 1;
        if(window.GetKey(GLFW_KEY_A) == GLFW_PRESS) camMoveVec.x += 1;
        if(window.GetKey(GLFW_KEY_D) == GLFW_PRESS) camMoveVec.x -= 1;
        if(window.GetKey(GLFW_KEY_Q) == GLFW_PRESS) camMoveVec.y += 1;
        if(window.GetKey(GLFW_KEY_E) == GLFW_PRESS) camMoveVec.y -= 1;


        glm::vec3 meshMoveVec(0, 0, 0);
        if(window.GetKey(GLFW_KEY_UP)    == GLFW_PRESS) meshMoveVec.z += 1;
        if(window.GetKey(GLFW_KEY_DOWN)  == GLFW_PRESS) meshMoveVec.z -= 1;
        if(window.GetKey(GLFW_KEY_LEFT)  == GLFW_PRESS) meshMoveVec.x += 1;
        if(window.GetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) meshMoveVec.x -= 1;
        if(window.GetKey(GLFW_KEY_Z)     == GLFW_PRESS) meshMoveVec.y += 1;
        if(window.GetKey(GLFW_KEY_C)     == GLFW_PRESS) meshMoveVec.y -= 1;

        meshPos += meshMoveVec * delta * 10.f;

        current->mMat = glm::translate(glm::mat4(1), meshPos);

        if(window.GetKey(GLFW_KEY_R) == GLFW_PRESS)
        {
            meshRot += -50.f * delta;
        }
        current->mMat = glm::rotate(current->mMat,
                glm::radians(meshRot),
                glm::vec3(0.0f, 1.0f, 0.0f));

        // current->mMat = glm::translate(current->mMat, meshPos);

        // TODO:make a simple good transform component for every scene object
        if(glm::length(camMoveVec) > 0)
        {
            camMoveVec /= camMoveVec.length(); // normalize
            cameraPos += camMoveVec * delta * 10.f;

            // dont follow mesh
            camera->viewMat = glm::lookAt(cameraPos,
                    meshPos,
                    glm::vec3(0, 1, 0));

            camera->SetPerspective(60.f, window.Aspect());
        }

        current->Render(camera->ViewProj());

        return false;
    });

    window->Render();

    EngineClean();
    return 0;
}
