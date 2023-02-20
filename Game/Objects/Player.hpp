#ifndef PLAYER_H
#define PLAYER_H

#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Game/Game.hpp"

struct PlayerSettings
{
    float accel;
    float brake;
    float minFriction;
    float maxFriction;

    float maxSpeed; // forward speed
    float minSpeed; // reverse speed

    float maxTurn;
    float rotSpeed;
    float bodyRotSpeed;
};

class Player : public Object
{
    public:
        Player(World* world, std::string name, const std::string& model, Game* game, PlayerSettings settings);

        void Start() override;
        void Tick(float deltaTime) override;

        float GetFuel() const { return fuel; }
        float GetMaxFuel() const { return 20; }

        float GetHealth() const { return health; }
        float GetSpeed() const { return glm::length(velocity); }

    private:
        float fuel;
        float health;
        float speed;

        int lapsDone;
        int checkpointsCleared;

        Game* game;
        PlayerSettings settings;

        glm::vec3 velocity;
        float velRotation;
        float bodyRotation;
        float prevRotation;
        
        // only do collision check on objects in this range
        float collisionRadius;

        Object* carBody;
        std::vector<Object*> wheels;
        std::vector<Object*> boundaryColliders;
        std::vector<Object*> checkpoints;

        bool collided = false;
        Timer collisionTimer, timer;

        void Respawn();
        bool CheckWallCollision();
        bool CheckCheckpointCollision();
};

#endif
