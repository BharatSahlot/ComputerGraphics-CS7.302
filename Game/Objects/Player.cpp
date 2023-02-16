#include "Player.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_transform.hpp"

glm::vec3 CalcForward(glm::vec3 rotation)
{
    auto rX = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    auto rY = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
    auto rZ = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));
    auto mat = rZ * rY * rX;

    return mat * glm::vec4(0, 0, 1, 0);
}

glm::vec3 ClampMagnitude(glm::vec3 vec, float maxMag)
{
    float mag = glm::length(vec);
    if(mag > maxMag)
    {
        return (vec / mag) * maxMag;
    }
    return vec;
}

Player::Player(World* world, std::string name, const std::string& model, Game* game, PlayerSettings settings)
    : Object(world, name, model), game(game), settings(settings)
{
}

void Player::Start()
{
    velocity = rotation = glm::vec3(0, 0, 0);
}

void Player::Tick(float deltaTime)
{
    float delta = 0.f;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_W)) delta = settings.accel;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_S)) delta -= settings.brake;

    glm::vec3 forward = CalcForward(rotation);
    velocity += forward * delta * deltaTime;

    float speed = glm::sign(glm::dot(forward, velocity)) * glm::length(velocity);

    if(speed < 0)
    {
        velocity += forward * glm::clamp(settings.friction * deltaTime, 0.f, -speed);
    }
    else velocity -= forward * glm::clamp(settings.friction * deltaTime, 0.f, speed);

    if(speed < 0) velocity = ClampMagnitude(velocity, -settings.minSpeed);
    else velocity = ClampMagnitude(velocity, settings.maxSpeed);


    transform->SetWorldPosition(transform->GetWorldPosition() + velocity);
}
