#ifndef GAME_UI_H
#define GAME_UI_H

#include "Engine/UI/Bar.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Game/Objects/Player.hpp"

class GameUI : public UIObject
{
    public:
        GameUI(World* world, std::string name);
        void Tick(float deltaTime) override;

    private:
        Player* player;

        std::shared_ptr<Bar> healthBar;
        std::shared_ptr<Bar> fuelBar;
};

#endif
