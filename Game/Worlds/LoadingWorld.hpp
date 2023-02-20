#ifndef LOADING_WORLD_H
#define LOADING_WORLD_H

#include "Engine/World.hpp"
#include "Game/Game.hpp"
#include "Game/Objects/LoadingBar.hpp"

class LoadingWorld : public World
{
    public:
        LoadingWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;
        void Tick(float deltaTime) const override;

    private:
        std::shared_ptr<LoadingBar> bar;
        Game* game;
};

#endif
