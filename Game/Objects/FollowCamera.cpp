#include "FollowCamera.hpp"
#include "Engine/World.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <iostream>

FollowCamera::FollowCamera(World* world, std::string name, std::string player, glm::vec3 offset) : Camera(world, name)
{
    this->player = world->GetObjectByName<Object>(player);
    if(this->player == nullptr)
    {
        std::cerr << "Player " << player << " for follow camera " << name << " not found" << std::endl;
    }

    this->offset = offset;
}

void FollowCamera::Start()
{
    glm::vec3 playerPos = player->transform->GetWorldPosition() + glm::vec3(0, 15, 0);
    pos = player->transform->GetModelMatrix() * glm::vec4(offset, 1);

    front = glm::normalize(playerPos - pos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up    = glm::normalize(glm::cross(right, front));

    view = glm::lookAt(pos, playerPos, up);
}

void FollowCamera::Tick(float deltaTime)
{
    glm::vec3 playerPos = player->transform->GetWorldPosition() + glm::vec3(0, 15, 0);
    pos = player->transform->GetModelMatrix() * glm::vec4(offset, 1);

    front = glm::normalize(playerPos - pos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up    = glm::normalize(glm::cross(right, front));

    view = glm::lookAt(pos, playerPos, up);
}
