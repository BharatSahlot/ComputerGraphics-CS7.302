#include "Zapper.hpp"
#include "Engine/Object.hpp"
#include "Globals.hpp"

#include <iostream>

Zapper::Zapper() : Object(Plane, BasicTexMat)
{
    Shader* baseVs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* lightingfs = Shader::MakeShader("Shaders/lighting.fs", GL_FRAGMENT_SHADER);

    Texture* lightingTex = Texture::MakeTexture("Game/Assets/Lighting.png", GL_LINEAR);

    std::shared_ptr<Material> lightingMat(Material::MakeMaterial(baseVs, lightingfs));
    this->material = lightingMat;

    this->UseTexture(std::shared_ptr<Texture>(lightingTex));
}

void Zapper::Start()
{
    this->transform->SetLocalScale(glm::vec3(1, 3, 1));
    this->transform->SetWorldPosition(9, 5, 0.1);
    this->frame = 0;
    this->frameTimer.Start();
}

void Zapper::Tick(const Window &window, float deltaTime)
{
    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / 150;
    if(ind > 0)
    {
        this->frame = (this->frame + ind) % 8;
        frameTimer.Start();
    }
}

void Zapper::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!this->material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    this->material->Use();
    this->material->SetVec3("col", glm::vec3(0.3, 0.2, 0.8));
    this->material->SetInt("texXCount", 5);
    this->material->SetInt("texYCount", 3);
    this->material->SetInt("index", this->frame);

    for(size_t i = 0; i < this->textures.size(); i++)
    {
        this->textures[i]->Use(i);
    }

    for(size_t i = 0; i < this->textures.size(); i++)
    {
        std::string tex = "texture";
        tex.append(std::to_string(i + 1));
        this->material->SetInt(tex, i);
    }

    glm::vec2 texDims = textures[0]->GetDims();
    this->material->SetVec2("texDims", texDims);
    this->material->SetVec2("spriteDims", glm::vec2(texDims.x / 8.f, texDims.y));

    this->material->SetUniformMat4("view", viewMat);
    this->material->SetUniformMat4("proj", projMat);
    this->material->SetUniformMat4("model", this->transform->GetModelMatrix());

    this->mesh->Render();
}
