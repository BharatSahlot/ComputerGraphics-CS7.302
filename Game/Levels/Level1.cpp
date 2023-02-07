#include "Game/Objects/Background.hpp"
#include "Game/Objects/Text.hpp"
#include "Levels.hpp"

int Level1::Load()
{
    Background* bg1 = new Background("bg0", "Game/Assets/Level1/Background_0.png", -0.2f * settings.speedModifier, -0.3f);
    Background* bg2 = new Background("bg1", "Game/Assets/Level1/Background_1.png", -0.8f * settings.speedModifier, -0.1f);

    Text* text = new Text(0.81f, 0.1f, 1, glm::vec3(1, 1, 1));
    text->SetText("Level 1");
    text->transform->SetWorldPosition(0, 0, 0.5f);

    objects.insert(std::shared_ptr<Background>(bg1));
    objects.insert(std::shared_ptr<Background>(bg2));
    objects.insert(std::shared_ptr<Text>(text));

    auto enterText = std::shared_ptr<Text>(new Text(-0.2f, 0.9f, 1.75f, glm::vec3(1, 1, 1)));
    enterText->SetText("Level 1");
    enterText->transform->SetWorldPosition(0, 0, 0.5f);
    menuObjects.insert(enterText);

    Level::Load();
    return 0;
}
