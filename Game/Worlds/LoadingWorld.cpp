#include "LoadingWorld.hpp"
#include "Game/Objects/LoadingBar.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Worlds/StartMenuWorld.hpp"

LoadingWorld::LoadingWorld(std::shared_ptr<Window> window, Game* game)
    : World(window), game(game)
{
}

void LoadingWorld::Start()
{
    GetResourceManager().AddInResourceQueue<Material>("fontMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/text.fs"
    });

    GetResourceManager().AddInResourceQueue<Material>("barMat", ResourceLoadData<Material> {
        "Shaders/text.vs", "Shaders/UI/bar.fs"
    });

    GetResourceManager().AddInResourceQueue<Font>("font", ResourceLoadData<Font> {
        "Assets/font.ttf", 48, "fontMat"
    });

    GetResourceManager().StartLoading();
    GetResourceManager().Load();

    bar = InstantiateUIObject<LoadingBar>("LoadingBar", [=](std::string* str) -> float {
        return game->GetStartMenuWorld().GetResourceManager().GetLoadStatus(str);
    });

    auto camera = Instantiate<Camera>("camera", glm::vec3(0, 0, -10), glm::vec3(0, 0, 0));
    window->SetCamera(camera);
}

void LoadingWorld::Tick(float deltaTime) const
{
    if(bar->IsDone())
    {
        game->SetGameState(GameState::InMenu);
        return;
    }
    World::Tick(deltaTime);
}
