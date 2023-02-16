#ifndef PLAYER_H
#define PLAYER_H

#include "Engine/Object.hpp"
#include "Game/Game.hpp"

struct PlayerSettings
{
    float accel;
    float brake;
    float friction;

    float maxSpeed; // forward speed
    float minSpeed; // reverse speed

    float rotSpeed;
};

class Player : public Object
{
    public:
        Player(World* world, std::string name, const std::string& model, Game* game, PlayerSettings settings);

        void Start() override;
        void Tick(float deltaTime) override;

    private:
        Game* game;
        PlayerSettings settings;

        glm::vec3 velocity;
        glm::vec3 rotation;
};

#endif
