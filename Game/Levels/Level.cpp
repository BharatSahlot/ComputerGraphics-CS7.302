#include "Level.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "GLFW/glfw3.h"
#include "Game/Objects/Background.hpp"
#include "Game/Objects/Ground.hpp"
#include "Game/Objects/Text.hpp"
#include "Game/Objects/Zapper.hpp"
#include "Game/Player/Player.hpp"
#include "Globals.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <algorithm>
#include <iostream>
#include <string>

#include "Engine/Utils/Random.hpp"
#include "glm/trigonometric.hpp"

Level::Level(std::shared_ptr<Camera> camera, LevelSettings settings)
{
    this->camera = camera;
    this->settings = settings;
    zapperSpawnInterval = 4;
}

int Level::Load()
{
    auto player = std::shared_ptr<Player>(new Player(this));

    started = hasEnded = playerDied = false;
    float tileSize = 0.5f;
    Ground* ground = new Ground(tileSize, GROUND_HEIGHT / tileSize, 100, -2.f * settings.speedModifier);

    for(int i = 0; i < 5; i++)
    {
        Zapper* zapper = new Zapper(settings.zapperHeight, -2.f * settings.speedModifier, settings.zapperCol);
        zapperPool.push(std::shared_ptr<Zapper>(zapper));
    }

    coinsText = std::shared_ptr<Text>(new Text(-0.99f, 0.1f, 1, glm::vec3(1, 1, 1)));
    coinsText->SetText("Coins");
    coinsText->transform->SetWorldPosition(0, 0, 0.5f);

    distText = std::shared_ptr<Text>(new Text(-0.99f, 0.24f, 1, glm::vec3(1, 1, 1)));
    distText->SetText("Dist");
    distText->transform->SetWorldPosition(0, 0, 0.5f);

    objects.insert(coinsText);
    objects.insert(distText);

    objects.insert(player);
    objects.insert(std::shared_ptr<Ground>(ground));

    auto enterText = std::shared_ptr<Text>(new Text(-0.6f, 1.1f, 1.5f, glm::vec3(1, 1, 1)));
    enterText->SetText("Press enter to start the game.");
    enterText->transform->SetWorldPosition(0, 0, 0.5f);
    menuObjects.insert(enterText);

    return 0;
}

void Level::Start()
{
    inMenu = true;
    for(auto obj: menuObjects)
    {
        obj->Start();
    }
    fadeDir = 1.f;
}

void Level::StartGame()
{
    for(auto obj: objects)
    {
        obj->Start();
    }
    zapperSpawnTimer.Start();
    levelTimer.Start();
    inMenu = false;
    fadeDir = 1.f;
}

void Level::Tick(const Window& window, float deltaTime)
{
    if(levelEnded)
    {
        windowFade -= deltaTime;
        if(windowFade <= 0.f)
        {
            hasEnded = true;
            windowFade = 0.f;
        }
        return;
    }

    if(inMenu)
    {
        if(enterPressed)
        {
            windowFade -= deltaTime;
            if(windowFade <= 0.f)
            {
                StartGame();
            }
            return;
        }

        windowFade += fadeDir * deltaTime;
        if(windowFade >= 1.f) windowFade = 1.f;
        for(auto obj: menuObjects)
        {
            obj->Tick(window, deltaTime);
        }
        if(window.GetKey(GLFW_KEY_ENTER))
        {
            enterPressed = true;
        }
        return;
    }

    windowFade += fadeDir * deltaTime;
    if(windowFade >= 1.f) windowFade = 1.f;

    if(levelTimer.TimeSinceStart() >= settings.duration)
    {
        levelEnded = true;
        return;
    }

    float interval = settings.zapperSpawnInterval;
    interval += (1.f - 2 * Random::GetFloat()) * settings.zapperSpawnIntervalVar;
    if(zapperSpawnTimer.TimeSinceStart() >= interval)
    {
        if(!zapperPool.empty())
        {
            float top = CEILING - settings.zapperHeight / 2.f;
            float bot = GROUND + GROUND_HEIGHT + settings.zapperHeight / 2.f;
            float y = bot + ((top - bot) * Random::GetFloat());
            glm::vec3 pos = window.ViewportPointToWorld(glm::vec3(1, 0, 0));
            pos.x += 1;
            pos.y = y;
            pos.z = 0.3f;

            zapperSpawnTimer.Start();
            auto zapper = zapperPool.top();
            objects.insert(zapper);
            zapperActive.push_back(zapper);
            zapperPool.pop();

            zapper->Start();
            if((Random::GetInt() % 2) == 0)
            {
                float speed = settings.zapperRotSpeed + (1.f - 2.f * Random::GetFloat()) * settings.zapperRotSpeedVar;
                if(Random::GetInt() % 2) speed *= -1;
                zapper->SetRotSpeed(speed);
            } else
            {
                float speed = settings.zapperYSpeed + (-Random::GetFloat()) * settings.zapperYSpeedVar;
                zapper->SetYSpeed(speed);
            }
            zapper->transform->SetWorldPosition(pos);
        }
    }

    dist += settings.speedModifier * deltaTime;
    coinsText->SetText(std::to_string(0));
    distText->SetText(std::to_string((int)(dist * 100)));
    for(auto obj: objects)
    {
        obj->Tick(window, deltaTime);
    }

    for(size_t i = 0; i < zapperActive.size(); i++)
    {
        if(zapperActive[i]->transform->GetWorldPosition().x < -0.2f)
        {
            objects.erase(std::find(objects.begin(), objects.end(), zapperActive[i]));
            zapperPool.push(zapperActive[i]);
            zapperActive.erase(zapperActive.begin() + i);
            i--;
        }
    }
}

void Level::EndLevel()
{
    playerDied = true;
    levelEnded = true;
    // hasEnded = true;
    // hasEnded = true;
}

int Level::Unload()
{
    objects.clear();
    return 0;
}

void Level::Render(const glm::mat4 &view, const glm::mat4 &proj)
{
    if(inMenu)
    {
        for(auto obj: menuObjects)
        {
            obj->Render(view, proj);
        }
        return;
    }

    for(auto obj: objects)
    {
        obj->Render(view, proj);
    }
}
