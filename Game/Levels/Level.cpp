#include "Level.hpp"
#include "Game/Player/Player.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

Level::Level(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

int Level::Load()
{
    Player* player = new Player;
    this->objects.push_back(std::shared_ptr<Player>(player));
    return 0;
}

void Level::Start()
{
    for(auto obj: this->objects)
    {
        obj->Start();
    }
}

void Level::Tick(float deltaTime)
{
    for(auto obj: this->objects)
    {
        obj->Tick(deltaTime);
    }
}

int Level::Unload()
{
    this->objects.clear();
    return 0;
}

void Level::Render(const glm::mat4 &view, const glm::mat4 &proj)
{
    for(auto obj: this->objects)
    {
        obj->Render(view, proj);
    }
}
