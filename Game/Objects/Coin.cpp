#include "Coin.hpp"
#include "Globals.hpp"

#include <iostream>

Coin::Coin(float radius, float speed) : Object("coin", Plane, SpriteSheetMat)
{
    this->speed = speed;

    sheet = Texture::MakeTexture("Game/Assets/coins.png");
    if(sheet == nullptr)
    {
        std::cerr << "Unable to load coin texture sheet" << std::endl;
    }

    transform->SetLocalScale(glm::vec3(radius, radius, 1.f));
}

void Coin::Start()
{
    frame = 0;
    frameTimer.Start();
}

void Coin::Tick(const Window &window, float deltaTime)
{
    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / SPRITE_ANIM_TIME_PER_FRAME;
    if(ind > 0)
    {
        frame = (frame + ind) % 8;
        frameTimer.Start();
    }

    glm::vec3 offset = glm::vec3(speed, 0, 0) * deltaTime;
    transform->SetWorldPosition(transform->GetWorldPosition() + offset);
}

void Coin::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    sheet->Use(0);

    material->Use();
    material->SetInt("texXCount", 8);
    material->SetInt("texYCount", 1);
    material->SetInt("index", frame);

    material->SetInt("texture1", 0);

    glm::vec2 texDims = sheet->GetDims();
    material->SetVec2("texDims", texDims);
    material->SetVec2("spriteDims", glm::vec2(texDims.x / 8.f, texDims.y));

    material->SetUniformMat4("view", viewMat);
    material->SetUniformMat4("proj", projMat);
    material->SetUniformMat4("model", transform->GetModelMatrix());

    mesh->Render();
}
