#include "GameWorld.hpp"
#include "Engine/Window/Window.hpp"

GameWorld::GameWorld(std::shared_ptr<Window> window, Game* game) : World(window)
{
    GetResourceManager().AddInResourceQueue("RaceTrack", ResourceLoadData<Model> {
        "Assets/race_track_1.fbx"
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

    camera = Instantiate<Camera>("camera", glm::vec3(0, 15, -50), glm::vec3(0, 0, 0));
    camera->clearColor = glm::vec3(21.f, 154.f, 198.f) / 255.f;
    camera->canMove = true;
}

void GameWorld::Start()
{
    GetResourceManager().Load();
    window->SetCamera(camera);

    Instantiate<Object>("raceTrack", "RaceTrack");

    Object* obj = GetObjectByName<Object>("Start");
    auto car = Instantiate<Object>("playerCar", "car");
    car->transform->SetLocalPosition(obj->transform->GetWorldPosition());
}

void GameWorld::Render()
{
    World::Render();
    sky->Render(camera->View(), camera->Proj());
}
