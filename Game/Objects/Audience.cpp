#include "Audience.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/Utils/Random.hpp"
#include "Engine/World.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>
#include <iostream>

Audience::Audience(World* world, std::string name, int count, float size) : Object(world, name), count(count), size(size)
{
    std::vector<float> vertices({
         0.5f,  0.5f, 0.0f, 1.f, 1.f, // top right
         0.5f, -0.5f, 0.0f, 1.f, 0.f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.f, 1.f // top left 
    });
    std::vector<unsigned int> indices({  // note that we start from 0!
        3, 1, 0,  // first Triangle
        3, 2, 1   // second Triangle
    });

    mesh = new Mesh(vertices, indices);

    // this is wrong, objects are instantiated in Start(), they should be instantiated in world constructor so we can use AddInResourceQueue
    material = world->GetResourceManager().AddInResourceQueue("audience_mat", ResourceLoadData<Material> { "Shaders/billboard.vs", "Shaders/spritesheet.fs" });

    spriteSheet = world->GetResourceManager().AddInResourceQueue("audience_tex", ResourceLoadData<Texture> { "Assets/audience.png", GL_LINEAR });
}

void Audience::Start()
{
    std::vector<Object*> spawnPoints = world->GetObjectsByPrefix<Object>("AudienceSpawn");
    count = std::min(count, (int)spawnPoints.size());

    frames.resize(count);
    Random::Shuffle(spawnPoints.begin(), spawnPoints.end());
    for(int i = 0; i < count; i++)
    {
        positions.push_back(spawnPoints[i]->transform->GetWorldPosition() + glm::vec3(0, (size / 2) - 5.f, 0));
        frames[i] = Random::GetInt() % 30;
    }

    frameTimer.Start();
}

void Audience::Render(const glm::mat4 &view, const glm::mat4 &proj)
{
    glm::vec3 cameraRight(view[0][0], view[1][0], view[2][0]);

    spriteSheet->Use(0);

    material->Use();
    material->SetVec3("cameraRight", cameraRight);
    material->SetVec3("cameraUp", glm::vec3(0, 1, 0));
    material->SetFloat("size", size);
    material->SetUniformMat4("model", glm::mat4(1));
    material->SetUniformMat4("view", view);
    material->SetUniformMat4("proj", proj);
    material->SetInt("texture1", 0);

    material->SetInt("index", 0);
    material->SetInt("texXCount", 6);
    material->SetInt("texYCount", 5);

    material->SetVec2("texDims", spriteSheet->GetDims());
    material->SetVec2("spriteDims", spriteSheet->GetDims() / glm::vec2(6, 5));

    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / 100.f;
    if(ind > 0)
    {
        frameTimer.Start();
    }

    for(int i = 0; i < count; i++)
    {
        material->Use();
        material->SetVec3("pos", positions[i]);
        if(ind > 0) frames[i] = (frames[i] + 1) % 30;
        material->SetInt("index", frames[i]);
        mesh->Render();
    }
}
