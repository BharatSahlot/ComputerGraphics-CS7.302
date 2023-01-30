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

#include <algorithm>
#include <iostream>

#include "Engine/Utils/Random.hpp"
#include "glm/trigonometric.hpp"

Level::Level(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
    zapperSpawnInterval = 4;
}

int Level::Load()
{
    Player* player = new Player;

    float tileSize = 0.5f;
    Ground* ground = new Ground(tileSize, GROUND_HEIGHT / tileSize, 100, -2.f);

    for(int i = 0; i < 5; i++)
    {
        Zapper* zapper = new Zapper(2.5f, -2.f);
        zapperPool.push(std::shared_ptr<Zapper>(zapper));
    }

    Background* bg1 = new Background("bg0", "Game/Assets/Background_0.png", -0.2f, -0.3f);
    Background* bg2 = new Background("bg1", "Game/Assets/Background_1.png", -0.8f, -0.1f);

    objects.insert(std::shared_ptr<Background>(bg1));
    objects.insert(std::shared_ptr<Background>(bg2));

    objects.insert(std::shared_ptr<Player>(player));
    objects.insert(std::shared_ptr<Ground>(ground));

    return 0;
}

void Level::Start()
{
    for(auto obj: objects)
    {
        obj->Start();
    }
    zapperSpawnTimer.Start();
}

void Level::Tick(const Window& window, float deltaTime)
{
    if(zapperSpawnTimer.TimeSinceStart() >= zapperSpawnInterval + (1.f - 2 * Random::GetFloat()) * (zapperSpawnInterval / 2.f))
    {
        if(!zapperPool.empty())
        {
            float top = 0.8f;
            float bot = -0.55f;
            float y = bot + ((top - bot) * Random::GetFloat());
            glm::vec3 pos = window.ViewportPointToWorld(glm::vec3(1, y, -0.3));
            pos.x += 1;

            glm::vec3 rot(0, 0, glm::radians((1.f - 2 * Random::GetFloat()) * 30.f));

            zapperSpawnTimer.Start();
            auto zapper = zapperPool.top();
            objects.insert(zapper);
            zapperActive.push_back(zapper);
            zapperPool.pop();

            zapper->Start();
            if((Random::GetInt() % 2) == 0)
            {
                zapper->SetRotSpeed(20.f + Random::GetFloat() * 20.f);
            } else
            {
                zapper->SetYSpeed(Random::GetFloat() * 2.f);
            }
            zapper->transform->SetWorldPosition(pos);
        }
    }

    for(auto obj: objects)
    {
        obj->Tick(window, deltaTime);
    }

    for(size_t i = 0; i < zapperActive.size(); i++)
    {
        if(zapperActive[i]->transform->GetWorldPosition().x < 0)
        {
            objects.erase(std::find(objects.begin(), objects.end(), zapperActive[i]));
            zapperPool.push(zapperActive[i]);
            zapperActive.erase(zapperActive.begin() + i);
            i--;
        }
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
