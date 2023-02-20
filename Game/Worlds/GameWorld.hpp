#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "Engine/UI/Text.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/World.hpp"
#include "Game/Game.hpp"
#include "Game/Objects/FollowCamera.hpp"
#include "Game/Objects/MapCamera.hpp"
#include "Game/Objects/Player.hpp"

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

        std::shared_ptr<Player> player;
        std::shared_ptr<Text> speedText;

        std::vector<Object*> checkpoints;
        std::vector<std::shared_ptr<Object>> fuelcans;

        // center and plane normal normal
        std::vector<std::pair<glm::vec3, glm::vec3>> fuelcanSpawnLines;

        Timer startTimer;

        void OnCheckPointReached(int checkpointReached);
        void OnFuelcanCollision(Object* can);
};

#endif
