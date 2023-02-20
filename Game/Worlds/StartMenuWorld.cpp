#include "StartMenuWorld.hpp"
#include "Engine/UI/Text.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Engine/Window/Window.hpp"
#include "GLFW/glfw3.h"
#include "Game/Objects/LoadingBar.hpp"
#include "Game/Worlds/GameWorld.hpp"

StartMenuWorld::StartMenuWorld(std::shared_ptr<Window> window, Game* game) : World(window), game(game)
{
    GetResourceManager().AddInResourceQueue("textMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    GetResourceManager().AddInResourceQueue<Material>("barMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/UI/bar.fs"
    });

    GetResourceManager().AddInResourceQueue("font", ResourceLoadData<Font> {
        "Assets/font.ttf", 48, "textMat"
    });

    GetResourceManager().AddInResourceQueue("zeyada", ResourceLoadData<Font> {
        "Assets/zeyada.ttf", 96, "textMat"
    });

    GetResourceManager().AddInResourceQueue("car", ResourceLoadData<Model> {
        // "Car/sportste.fbx"
        "Assets/Car.fbx"
    });

    camera = Instantiate<Camera>("camera", glm::vec3(0, 25, -100), glm::vec3(0, 5, 0));
    // camera->clearColor = glm::vec3(21.f, 154.f, 198.f) / 255.f;
    camera->clearColor = glm::vec3(0.1f, 0.1f, 0.1f);
}

void StartMenuWorld::Start()
{
    // join thread and finish loading
    GetResourceManager().Load();

    window->SetCamera(camera);

    InstantiateUIObject<LoadingBar>("LoadingBar", [=](std::string* str) -> float {
        return game->GetGameWorld().GetResourceManager().GetLoadStatus(str);
    });

    InstantiateUIObject<Text>("title", "zeyada", Anchor {
        AnchorType::Center,
        glm::vec2(0, 150),
        0.6f
    })
    ->SetText("Racing Game")
    ->EnableShadow()
    ->SetShadow(0.05f, glm::vec4(0.8, 0.3, 0.1, 0.3));

    InstantiateUIObject<Text>("enterToPlayTxt", "font", Anchor {
        AnchorType::Center,
        glm::vec2(0, -150),
        0.4f
    })
    ->SetText("Press enter to start the game");

    InstantiateUIObject<Text>("controlsTxt", "font", Anchor {
        AnchorType::Center,
        glm::vec2(0, -170),
        0.4f
    })
    ->SetText("Use WASD to control car. R to toggle the camera view.");

    car = Instantiate<Object>("car", "car");

    World::Start();
}

void StartMenuWorld::Tick(float deltaTime) const
{
    float y = glm::degrees(car->transform->GetLocalRotation().y) + deltaTime * 50.f;
    car->transform->SetLocalRotation(glm::vec3(0, glm::radians(y), 0));

    if(game->GetGameWorld().GetResourceManager().HasLoadingFinished() && window->GetKeyDown(GLFW_KEY_ENTER))
    {
        game->SetGameState(GameState::InGame);
    }

    World::Tick(deltaTime);
}

void StartMenuWorld::Render()
{
    World::Render();
}
