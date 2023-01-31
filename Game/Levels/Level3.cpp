#include "Game/Objects/Background.hpp"
#include "Game/Objects/Text.hpp"
#include "Levels.hpp"
#include <string>

int Level3::Load()
{
    Level::Load();

    for(int i = 2; i <= 8; i++)
    {
        std::string file = "Game/Assets/Level2/";
        file += std::to_string(i);
        file += ".png";

        Background* bg = new Background("bg0", file, -0.2f * ((float)i / 2.f) * settings.speedModifier, -1.f / i);

        objects.insert(std::shared_ptr<Background>(bg));
    }

    Text* text = new Text(0.81f, 0.1f, 1, glm::vec3(1, 1, 1));
    text->SetText("Level 3");
    text->transform->SetWorldPosition(0, 0, 0.5f);
    objects.insert(std::shared_ptr<Text>(text));
    return 0;
}
