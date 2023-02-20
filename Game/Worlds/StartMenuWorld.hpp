#ifndef START_MENU_WORLD_H
#define START_MENU_WORLD_H

#include "Engine/World.hpp"
#include "Game/Game.hpp"

class StartMenuWorld : public World
{
    public:
        StartMenuWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;
        void Tick(float deltaTime) const override;
        void Render() override;

    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Object> car;
        Game* game;
};

#endif
