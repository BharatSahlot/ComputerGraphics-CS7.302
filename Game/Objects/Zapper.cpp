#include "Zapper.hpp"
#include "Engine/Object.hpp"
#include "Globals.hpp"

#include <iostream>

Zapper::Zapper(float height, float speed, glm::vec3 col) : Object("zapper", Plane, BasicTexMat)
{
    Shader* baseVs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* lightingfs = Shader::MakeShader("Shaders/lighting.fs", GL_FRAGMENT_SHADER);

    Texture* lightingTex = Texture::MakeTexture("Game/Assets/Lighting.png", GL_LINEAR);

    std::shared_ptr<Material> lightingMat(Material::MakeMaterial(baseVs, lightingfs));
    material = lightingMat;

    UseTexture(std::shared_ptr<Texture>(lightingTex));
    this->height = height;
    this->xSpeed = speed;
    this->ySpeed = 0;
    this->col = col;
}

void Zapper::SetYSpeed(float ySpeed)
{
    this->defYSpeed = ySpeed;
    this->ySpeed = ySpeed;
}

void Zapper::SetRotSpeed(float speed)
{
    rSpeed = speed;
}

void Zapper::Start()
{
    transform->SetLocalScale(glm::vec3(height / 3, height, 1));
    transform->SetLocalRotation(glm::vec3(0));
    frame = 0;
    frameTimer.Start();
    rSpeed = defYSpeed = ySpeed = 0.f;
}

void Zapper::Tick(const Window &window, float deltaTime)
{
    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / 150;
    if(ind > 0)
    {
        frame = (this->frame + ind) % 8;
        frameTimer.Start();
    }

    if(transform->GetLocalPosition().y - height / 2.f <= GROUND + GROUND_HEIGHT) ySpeed = defYSpeed;
    if(transform->GetLocalPosition().y + height / 2.f >= CEILING) ySpeed = -defYSpeed;

    auto offset = glm::vec3(xSpeed, ySpeed, 0) * deltaTime;
    transform->SetWorldPosition(transform->GetWorldPosition() + offset);

    auto rOffset = glm::vec3(0, 0, glm::radians(rSpeed)) * deltaTime;
    transform->SetLocalRotation(transform->GetLocalRotation() + rOffset);
    transform->SetLocalScale(glm::vec3(height / 3, height, 1));
}

void Zapper::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    material->Use();
    material->SetVec3("col", col);
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
