#ifndef GAME_OVER_WORLD_H
#define GAME_OVER_WORLD_H

#include "Engine/World.hpp"
#include "Game/Game.hpp"

class GameOverWorld : public World
{
    public:
        GameOverWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;

    private:
        std::shared_ptr<Camera> camera;
        Game* game;
};

#endif
