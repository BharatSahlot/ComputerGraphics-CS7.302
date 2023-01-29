#include "Level.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include "Game/Objects/Background.hpp"
#include "Game/Objects/Ground.hpp"
#include "Game/Objects/Zapper.hpp"
#include "Game/Player/Player.hpp"
#include "Globals.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

Level::Level(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

int Level::Load()
{
    Player* player = new Player;

    float tileSize = 0.5f;
    Ground* ground = new Ground(tileSize, GROUND_HEIGHT / tileSize, 100, -1.f);

    Zapper* zapper = new Zapper(1.5f);

    Background* bg1 = new Background("bg0", "Game/Assets/Background_0.png", -0.2f, -0.3f);
    Background* bg2 = new Background("bg1", "Game/Assets/Background_1.png", -0.8f, -0.1f);

    objects.push_back(std::shared_ptr<Background>(bg1));
    objects.push_back(std::shared_ptr<Background>(bg2));

    objects.push_back(std::shared_ptr<Player>(player));
    objects.push_back(std::shared_ptr<Zapper>(zapper));
    objects.push_back(std::shared_ptr<Ground>(ground));

    return 0;
}

void Level::Start()
{
    for(auto obj: objects)
    {
        obj->Start();
    }
}

void Level::Tick(const Window& window, float deltaTime)
{
    for(auto obj: objects)
    {
        obj->Tick(window, deltaTime);
    }
}

int Level::Unload()
{
    objects.clear();
    return 0;
}

void Level::Render(const glm::mat4 &view, const glm::mat4 &proj)
{
    for(auto obj: objects)
    {
        obj->Render(view, proj);
    }
}
