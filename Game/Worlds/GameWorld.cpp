#include "GameWorld.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_transform.hpp"

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

    mapCamera = Instantiate<MapCamera>("mapCamera", "playerCar", glm::vec3(5, 100, 5));
    mapCamera->clearColor = glm::vec3(0, 0, 0);
}

void GameWorld::Render()
{
    float cz = camera->Position().z;
    std::sort(objects.begin(), objects.end(), [cz](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool {
        bool ta = a->IsTransparent();
        bool tb = b->IsTransparent();

        // a is transparent b is not
        if(ta && !tb) return false;

        // a is not b is
        if(!ta && tb) return true;

        // a and b both(not)
        float az = std::abs(cz - a->transform->GetWorldPosition().z);
        float bz = std::abs(cz - b->transform->GetWorldPosition().z);
        return az < bz;
    });

    camera->Use(glm::vec2(window->Width(), window->Height()), glm::vec3(0, 0, 1.f));
    for(auto x: objects) x->Render(camera->View(), camera->Proj());
    for(auto x: uiObjs) x->Render();

    glm::vec3 position = GetObjectByName<Object>("playerCar")->transform->GetWorldPosition();
    glm::vec3 rotation = GetObjectByName<Object>("playerCar")->transform->GetLocalRotation();
    auto rX = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    auto rY = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
    auto rZ = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));
    auto mat = rZ * rY * rX;

    glm::vec3 forward = glm::normalize(mat * glm::vec4(0, 0, 1, 0));
    position.y += 40.f;
    // std::cout << position.x << ' ' << position.y << ' ' << position.z << std::endl;
    primitive->DrawLine(position, position + forward * 30.f, glm::vec3(1, 0, 0));

    sky->Render(camera->View(), camera->Proj());

    mapCamera->SetPerspective(90.f, window->Aspect());
    mapCamera->Use(glm::vec2(window->Width(), window->Height()), glm::vec3(15, -15, 0.2f));
    for(auto x: objects) x->Render(mapCamera->View(), mapCamera->Proj());
}
