#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "Engine/Object.hpp"
#include "Engine/World.hpp"
#include "Game/Game.hpp"
#include "Game/Objects/Player.hpp"

class AIPlayer : public Object
{
    public:
        AIPlayer(World* world, std::string name, std::string model, Game* game, PlayerSettings settings, glm::vec3 spawnOffset);

        void Start() override;
        void Tick(float deltaTime) override;

    private:
        Game* game;
        PlayerSettings settings;

        int checkpointsCleared;
        int lapsDone;

        glm::vec3 targetPos;
        glm::vec3 spawnOffset;

        glm::vec3 velocity;
        float velRotation;

        // only do collision check on objects in this range
        float collisionRadius;

        Object* carBody;
        std::vector<Object*> boundaryColliders;
        std::vector<Object*> checkpoints;

        std::vector<std::pair<glm::vec3, glm::vec3>> targetLines;

        bool collided = false;
        Timer collisionTimer, timer;

        glm::vec3 GetPointAlongCheckpoint(int c);

        void Respawn();
        bool CheckWallCollision();
        bool CheckCheckpointCollision();
};

#endif
