#include "GameWorld.hpp"
#include "Engine/Render/Model.hpp"
#include "Engine/Utils/Random.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Objects/Audience.hpp"
#include "Game/Objects/CountdownText.hpp"
#include "Game/Objects/GameUI.hpp"
#include "Game/Objects/Player.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

GameWorld::GameWorld(std::shared_ptr<Window> window, Game* game) : World(window)
{
    GetResourceManager().AddInResourceQueue("textMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    GetResourceManager().AddInResourceQueue("zeyada", ResourceLoadData<Font> {
        "Assets/zeyada.ttf", 96, "textMat"
    });

    GetResourceManager().AddInResourceQueue("gajraj", ResourceLoadData<Font> {
        "Assets/gajraj.ttf", 96, "textMat"
    });

    GetResourceManager().AddInResourceQueue("font", ResourceLoadData<Font> {
        "Assets/font.ttf", 96, "textMat"
    });

    GetResourceManager().AddInResourceQueue<Material>("barMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/UI/bar.fs"
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

    GetResourceManager().AddInResourceQueue("jerrycan", ResourceLoadData<Model> {
        "Assets/jerrycan.fbx"
    });

    Instantiate<Audience>("audience", 50, 50);
}

void GameWorld::Start()
{
    GetResourceManager().Load();

    Instantiate<Object>("raceTrack", "RaceTrack");

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
    player->onCheckPointReached = [this](int c) { this->OnCheckPointReached(c); };
    player->onFuelcanCollision = [this](Object* can) { this->OnFuelcanCollision(can); };

    checkpoints = GetObjectsByPrefix<Object>("Checkpoint");
    std::sort(checkpoints.begin(), checkpoints.end(), [](Object* a, Object* b) {
        return a->name < b->name;
    });

    for(int i = 0; i < checkpoints.size(); i++)
    {
        auto can = Instantiate<Object>("FuelCan." + std::to_string(i), "jerrycan");
        can->SetActive(i != 0 && i <= 3);
        can->transform->SetLocalScale(glm::vec3(0.3));
        fuelcans.push_back(can);

        auto cpoint = checkpoints[i];
        const std::vector<float>& vertices = cpoint->GetMeshes()[0]->GetVertices();
        const std::vector<unsigned int>& faces = cpoint->GetMeshes()[0]->GetFaces();

        glm::mat4 mat = cpoint->transform->GetModelMatrix();
        glm::vec3 a = mat * glm::vec4(vertices[8 * faces[0]], 
                vertices[8 * faces[0] + 1],
                vertices[8 * faces[0] + 2], 1);

        glm::vec3 b = mat * glm::vec4(vertices[8 * faces[1]],
                vertices[8 * faces[1] + 1],
                vertices[8 * faces[1] + 2], 1);

        glm::vec3 c = mat * glm::vec4(vertices[8 * faces[2]],
                vertices[8 * faces[2] + 1],
                vertices[8 * faces[2] + 2], 1);

        glm::vec3 norm = glm::normalize(glm::cross(
                        glm::normalize(b - a),
                        glm::normalize(c - a)
                    ));

        glm::vec3 normnorm = glm::normalize(glm::cross(norm, glm::vec3(0, 1, 0)));
        fuelcanSpawnLines.push_back({ cpoint->transform->GetWorldPosition() * glm::vec3(1, 0, 1), normnorm });

        glm::vec3 pos = fuelcanSpawnLines[i].first + (1.f - 2.f * Random::GetFloat()) * 75.f * fuelcanSpawnLines[i].second;

        pos.y += 15.f;
        can->transform->SetWorldPosition(pos);
        can->transform->SetLocalRotation(glm::vec3(glm::radians(45.f), Random::GetFloat(), 0));
    }

    camera = Instantiate<FollowCamera>("camera", "playerCar", glm::vec3(0, 400, -750));
    camera->clearColor = glm::vec3(0);
    camera->canMove = true;

    mapCamera = Instantiate<MapCamera>("mapCamera", "playerCar", glm::vec3(5, 100, 5));
    mapCamera->clearColor = glm::vec3(0, 0, 0);

    window->SetCamera(camera);

    InstantiateUIObject<CountdownText>("countdownText", 3.f);
    InstantiateUIObject<GameUI>("GameUI");

    speedText = InstantiateUIObject<Text>("speedText", "gajraj", Anchor {
        AnchorType::BottomRight,
        glm::vec2(15, 15),
        0.5
    });
    speedText->SetShadow(0.1, glm::vec4(0, 0, 0, 1));
    speedText->EnableShadow();

    startTimer.Start();
    World::Start();
}

void GameWorld::Tick(float deltaTime) const
{
    for(auto x: fuelcans)
    {
        if(!x->IsActive()) continue;
        float y = glm::degrees(x->transform->GetLocalRotation().y);
        y += 80.f * deltaTime;
        x->transform->SetLocalRotation(glm::vec3(glm::radians(45.f), glm::radians(y), 0));
    }

    if(startTimer.TimeSinceStart() >= 3.f) for(auto x: objects) x->Tick(deltaTime);

    int speed = glm::round(player->GetSpeed() * 20);
    speedText->SetText(std::to_string(speed));

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

void GameWorld::OnCheckPointReached(int checkpointReached)
{
    int c = (checkpointReached + 3) % checkpoints.size();
    fuelcans[c]->SetActive(true);

    glm::vec3 pos = fuelcanSpawnLines[c].first + (1.f - 2.f * Random::GetFloat()) * 75.f * fuelcanSpawnLines[c].second;

    pos.y += 15.f;
    fuelcans[c]->transform->SetWorldPosition(pos);
}

void GameWorld::OnFuelcanCollision(Object* can)
{
    can->SetActive(false);
}
