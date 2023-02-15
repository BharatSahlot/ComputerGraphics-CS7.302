#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "Engine/World.hpp"
#include "Game/Game.hpp"

class GameWorld : public World
{
    public:
        GameWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;
        // void Tick(float deltaTime) const override;
        void Render() override;

    private:
        Game* game;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Camera> mapCamera;
        std::shared_ptr<Sky> sky;
};

#endif
