#include "Game/Objects/Background.hpp"
#include "Game/Objects/Text.hpp"
#include "Levels.hpp"
#include <memory>
#include <string>

int Level2::Load()
{
    Level::Load();
    Background* bg1 = new Background("bg0", "Game/Assets/Level1/Background_0.png", -0.2f * settings.speedModifier, -0.3f);
    Background* bg2 = new Background("bg1", "Game/Assets/Level1/Background_1.png", -0.8f * settings.speedModifier, -0.1f);

    Text* text = new Text(0.81f, 0.1f, 1, glm::vec3(1, 1, 1));
    text->SetText("Level 2");
    text->transform->SetWorldPosition(0, 0, 0.5f);
    objects.insert(std::shared_ptr<Text>(text));

    objects.insert(std::shared_ptr<Background>(bg1));
    objects.insert(std::shared_ptr<Background>(bg2));
    return 0;
}
