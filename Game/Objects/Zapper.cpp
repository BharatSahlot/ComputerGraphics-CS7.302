#include "Zapper.hpp"
#include "Engine/Object.hpp"
#include "Globals.hpp"

#include <iostream>

Zapper::Zapper(float height) : Object("zapper", Plane, BasicTexMat)
{
    Shader* baseVs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* lightingfs = Shader::MakeShader("Shaders/lighting.fs", GL_FRAGMENT_SHADER);

    Texture* lightingTex = Texture::MakeTexture("Game/Assets/Lighting.png", GL_LINEAR);

    std::shared_ptr<Material> lightingMat(Material::MakeMaterial(baseVs, lightingfs));
    material = lightingMat;

    UseTexture(std::shared_ptr<Texture>(lightingTex));
    this->height = height;
}

void Zapper::Start()
{
    transform->SetLocalScale(glm::vec3(height / 3, height, 1));
    transform->SetWorldPosition(9, 5, 0.1);
    frame = 0;
    frameTimer.Start();
}

void Zapper::Tick(const Window &window, float deltaTime)
{
    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / 150;
    if(ind > 0)
    {
        frame = (this->frame + ind) % 8;
        frameTimer.Start();
    }
}

void Zapper::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    material->Use();
    material->SetVec3("col", glm::vec3(0.3, 0.2, 0.8));
    material->SetInt("texXCount", 5);
    material->SetInt("texYCount", 3);
    material->SetInt("index", frame);

    for(size_t i = 0; i < textures.size(); i++)
    {
        textures[i]->Use(i);
    }

    for(size_t i = 0; i < textures.size(); i++)
    {
        std::string tex = "texture";
        tex.append(std::to_string(i + 1));
        material->SetInt(tex, i);
    }

    glm::vec2 texDims = textures[0]->GetDims();
    material->SetVec2("texDims", texDims);
    material->SetVec2("spriteDims", glm::vec2(texDims.x / 8.f, texDims.y));

    material->SetUniformMat4("view", viewMat);
    material->SetUniformMat4("proj", projMat);
    material->SetUniformMat4("model", transform->GetModelMatrix());

    mesh->Render();
}
