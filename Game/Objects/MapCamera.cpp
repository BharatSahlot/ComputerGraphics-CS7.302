#include "MapCamera.hpp"
#include "Engine/World.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <iostream>

MapCamera::MapCamera(World* world, std::string name, std::string player, glm::vec3 offset) : Camera(world, name)
{
    this->player = world->GetObjectByName<Object>(player);
    if(this->player == nullptr)
    {
        std::cerr << "Player " << player << " for map camera " << name << " not found" << std::endl;
    }

    this->offset = offset;
}

void MapCamera::Start()
{
    glm::vec3 playerPos = player->transform->GetWorldPosition();
    pos = playerPos + offset;

    front = glm::normalize(playerPos - pos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up    = glm::normalize(glm::cross(right, front));

    view = glm::lookAt(pos, pos + front, up);
}

void MapCamera::Tick(float deltaTime)
{
    glm::vec3 playerPos = player->transform->GetWorldPosition();
    pos = playerPos + offset;

    front = glm::normalize(playerPos - pos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up    = glm::normalize(glm::cross(right, front));

    view = glm::lookAt(pos, pos + front, up);
}
