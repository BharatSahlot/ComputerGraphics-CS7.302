#include "Player.hpp"
#include "Engine/Object.hpp"

#include "Globals.hpp"
#include <iostream>

Player::Player() : Object(Plane, BasicMat)
{
    this->material = BasicMat;
}


void Player::Start()
{
    this->transform->SetWorldPosition(0.f, 5.f, 0.f);
}

void Player::Tick(float deltaTime)
{
}

void Player::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    this->material->SetVec3("col", glm::vec3(0.5f, 0.2f, 0.3f));
    Object::Render(viewMat, projMat);
}
