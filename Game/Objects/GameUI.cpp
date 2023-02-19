#include "GameUI.hpp"
#include "Engine/World.hpp"

GameUI::GameUI(World* world, std::string name) : UIObject(world, name)
{
    player = world->GetObjectByName<Player>("playerCar");
    healthBar = world->InstantiateUIObject<Bar>(name + ":healthBar", Anchor {
        AnchorType::BottomLeft,
        glm::vec2(5, 5),
        1
    }, 0.4f, 5.f);
    healthBar->SetFillAmount(1);
    healthBar->SetColor(glm::vec3(0.1, 1, 0.3));
    healthBar->SetBorder(0.0);

    fuelBar = world->InstantiateUIObject<Bar>(name + ":fuelBar", Anchor {
        AnchorType::BottomLeft,
        glm::vec2(5, 10),
        1
    }, 0.25f, 3.5f);
    fuelBar->SetFillAmount(1);
    fuelBar->SetColor(glm::vec3(0, 0, 0));
    fuelBar->SetBorder(0.0);
}

void GameUI::Tick(float deltaTime)
{
    healthBar->SetFillAmount(player->GetHealth() / 100.f);
    fuelBar->SetFillAmount(player->GetFuel() / player->GetMaxFuel());
}
