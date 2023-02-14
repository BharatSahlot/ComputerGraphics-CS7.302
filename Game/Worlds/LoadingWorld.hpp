#ifndef LOADING_WORLD_H
#define LOADING_WORLD_H

#include "Engine/World.hpp"
#include "Game/Game.hpp"

class LoadingWorld : public World
{
    public:
        LoadingWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;
        void Tick(float deltaTime) const override;

    private:
        Game* game;
};

#endif
