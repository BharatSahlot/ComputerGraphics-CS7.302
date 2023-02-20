#include "GameOverWorld.hpp"
#include "Engine/Window/Window.hpp"

GameOverWorld::GameOverWorld(std::shared_ptr<Window> window, Game* game) : World(window), game(game)
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

    camera = Instantiate<Camera>("camera", glm::vec3(0, 5, -100), glm::vec3(0, 5, 0));
    camera->clearColor = glm::vec3(0.1f, 0.1f, 0.1f);
}

void GameOverWorld::Start()
{
    GetResourceManager().Load();

    window->SetCamera(camera);

    InstantiateUIObject<Text>("title", "font", Anchor {
        AnchorType::Center,
        glm::vec2(0, 150),
        0.6f
    })
    ->SetText(game->deathReason);
}
