#include "GameWorld.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Objects/Audience.hpp"
#include "Game/Objects/CountdownText.hpp"
#include "Game/Objects/Player.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

GameWorld::GameWorld(std::shared_ptr<Window> window, Game* game) : World(window)
{
    GetResourceManager().AddInResourceQueue("textMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    GetResourceManager().AddInResourceQueue("font", ResourceLoadData<Font> {
        "Assets/font.ttf", 96, "textMat"
    });

    GetResourceManager().AddInResourceQueue("RaceTrack", ResourceLoadData<Model> {
        "Assets/race_track_1-1.fbx"
    });

    GetResourceManager().AddInResourceQueue("car", ResourceLoadData<Model> {
        "Assets/Car.fbx"
    });

    sky = GetResourceManager().AddInResourceQueue("sky", ResourceLoadData<Sky> {
        {
            "Assets/skybox/right.png",
            "Assets/skybox/left.png",
            "Assets/skybox/top.png",
            "Assets/skybox/bottom.png",
            "Assets/skybox/front.png",
            "Assets/skybox/back.png",
        }
    });

    Instantiate<Audience>("audience", 50, 50);
}

void GameWorld::Start()
{
    GetResourceManager().Load();

    Instantiate<Object>("raceTrack", "RaceTrack");

    Object* obj = GetObjectByName<Object>("Checkpoint");

    player = Instantiate<Player>("playerCar", "car", game, PlayerSettings {
        20.f, // accel 
        10.f, // brake
        9.f, // min friction
        22.f, // max fricion
        6.f, // max speed
        -3.f, // min speed
        40.f, // maximum wheel angle
        70.f, // car wheel rotation speed
        65.f // car body rotation speed
    });
    player->transform->SetLocalPosition(obj->transform->GetWorldPosition() * glm::vec3(1, 0, 1));

    camera = Instantiate<FollowCamera>("camera", "playerCar", glm::vec3(0, 400, -750));
    // camera->clearColor = glm::vec3(21.f, 154.f, 198.f) / 255.f;
    camera->clearColor = glm::vec3(0);
    camera->canMove = true;

    mapCamera = Instantiate<MapCamera>("mapCamera", "playerCar", glm::vec3(5, 100, 5));
    mapCamera->clearColor = glm::vec3(0, 0, 0);

    window->SetCamera(camera);

    InstantiateUIObject<CountdownText>("countdownText", 3.f);

    startTimer.Start();
    World::Start();
}

void GameWorld::Tick(float deltaTime) const
{
    if(startTimer.TimeSinceStart() >= 3.f) for(auto x: objects) x->Tick(deltaTime);
    for(auto x: uiObjs) x->Tick(deltaTime);
}

void GameWorld::Render()
{
    glm::vec3 cz = camera->Position();
    std::sort(objects.begin(), objects.end(), [cz](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool {
        bool ta = a->IsTransparent();
        bool tb = b->IsTransparent();

        // a is transparent b is not
        if(ta && !tb) return false;

        // a is not b is
        if(!ta && tb) return true;

        // a and b both(not)
        float az = glm::distance2(cz, a->transform->GetWorldPosition());
        float bz = glm::distance2(cz, b->transform->GetWorldPosition());
        if(ta && tb) return az > bz;
        return az < bz;
    });

    camera->Use(glm::vec2(window->Width(), window->Height()), glm::vec3(0, 0, 1.f));

    for(auto x: objects)
    {
        if(x->IsTransparent()) break;
        x->Render(camera->View(), camera->Proj());
    }

    while(!lineQueue.empty())
    {
        auto [start, end, col] = lineQueue.front();
        lineQueue.pop();

        primitive->DrawLine(start, end, col);
    }

    while(!boxQueue.empty())
    {
        auto [center, extents, col] = boxQueue.front();
        boxQueue.pop();

        primitive->DrawBox(center, extents, col);
    }

    sky->Render(camera->View(), camera->Proj());

    for(auto x: objects)
    {
        if(!x->IsTransparent()) continue;
        x->Render(camera->View(), camera->Proj());
    }

    for(auto x: uiObjs) x->Render();

    mapCamera->SetPerspective(90.f, window->Aspect());
    mapCamera->Use(glm::vec2(window->Width(), window->Height()), glm::vec3(15, -15, 0.2f));
    for(auto x: objects) x->Render(mapCamera->View(), mapCamera->Proj());
}
