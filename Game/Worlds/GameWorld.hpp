#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "Engine/Utils/Timer.hpp"
#include "Engine/World.hpp"
#include "Game/Game.hpp"
#include "Game/Objects/FollowCamera.hpp"
#include "Game/Objects/MapCamera.hpp"

class GameWorld : public World
{
    public:
        GameWorld(std::shared_ptr<Window> window, Game* game);

        void Start() override;
        void Tick(float deltaTime) const override;
        void Render() override;

    private:
        Game* game;
        std::shared_ptr<FollowCamera> camera;
        std::shared_ptr<MapCamera> mapCamera;
        std::shared_ptr<Sky> sky;

        Timer startTimer;
};

#endif
