#include "Player.hpp"
#include "Engine/Object.hpp"

#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Globals.hpp"
#include "glm/common.hpp"
#include <iostream>

Player::Player() : Object(Plane, BasicMat)
{
    this->material = BasicMat;
}

void Player::Start()
{
    this->transform->SetWorldPosition(4.f, 9.f, 0.f);
}

void Player::Tick(const Window& window, float deltaTime)
{
    this->velocity.y += GRAVITY * deltaTime;

    if(window.GetKey(GLFW_KEY_SPACE))
    {
        this->velocity.y += NITRO * deltaTime;
    }

    this->velocity.y = glm::clamp(this->velocity.y, -MAX_Y_VELOCITY, MAX_Y_VELOCITY);

    glm::vec3 pos = this->transform->GetWorldPosition();
    pos += this->velocity * deltaTime;

    if(pos.y >= CEILING && this->velocity.y > 0)
    {
        pos.y = CEILING;
        this->velocity.y = 0;
    }

    if(pos.y <= GROUND + GROUND_HEIGHT && this->velocity.y < 0)
    {
        pos.y = GROUND + GROUND_HEIGHT;
        this->velocity.y = 0;
    }

    this->transform->SetWorldPosition(pos);
}

void Player::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    this->material->Use();
    this->material->SetVec3("col", glm::vec3(1.f, 1.f, 1.f));
    Object::Render(viewMat, projMat);
}
