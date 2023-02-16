#include "Player.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/vector_angle.hpp"

glm::vec3 CalcForward(glm::vec3 rotation)
{
    auto rX = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    auto rY = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
    auto rZ = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));
    auto mat = rZ * rY * rX;

    return glm::normalize(glm::vec3(mat * glm::vec4(0, 0, 1, 0)));
}

glm::vec3 ClampMagnitude(glm::vec3 vec, float minMag, float maxMag)
{
    float mag = glm::length(vec);
    if(mag > maxMag)
    {
        return (vec / mag) * maxMag;
    } else if(mag < minMag)
    {
        return (vec / mag) * minMag;
    }
    return vec;
}

float AbsMin(float a, float b)
{
    if(std::abs(a) < std::abs(b)) return a;
    return b;
}

Player::Player(World* world, std::string name, const std::string& model, Game* game, PlayerSettings settings)
    : Object(world, name, model), game(game), settings(settings)
{
}

void Player::Start()
{
    velocity = glm::vec3(0, 0, 0);
    velRotation = prevRotation = bodyRotation = 0;

    carBody = world->GetObjectByName<Object>("Body");
    wheels.push_back(world->GetObjectByName<Object>("WheelFL"));
    wheels.push_back(world->GetObjectByName<Object>("WheelFR"));
}

void Player::Tick(float deltaTime)
{
    float delta = 0.f;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_W)) delta = settings.accel;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_S)) delta -= settings.brake;

    if(world->GetWindow().GetKeyDown(GLFW_KEY_A)) velRotation += settings.rotSpeed * deltaTime;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_D)) velRotation -= settings.rotSpeed * deltaTime;

    velRotation = glm::clamp(velRotation, prevRotation - settings.maxTurn, prevRotation + settings.maxTurn);

    glm::vec3 forward = CalcForward(glm::vec3(0, glm::radians(velRotation), 0));
    velocity += forward * delta * deltaTime;

    float carBodyZAngle = 0.f;

    if(glm::epsilonNotEqual(0.f, glm::length(velocity), glm::epsilon<float>()))
    {
        glm::vec3 velNorm = glm::normalize(velocity);

        float speed = glm::length(velocity);
        carBodyZAngle = AbsMin(glm::orientedAngle(forward, velNorm, glm::vec3(0, 1, 0)),
                glm::orientedAngle(-forward, velNorm, glm::vec3(0, 1, 0))) / 5.f;

        float t = 1 - glm::abs(glm::dot(forward, velNorm));
        t = glm::clamp(t * 25, 0.f, 1.f);
        float frictionAmount = glm::mix(settings.minFriction, settings.maxFriction, t);

        glm::vec3 friction = -glm::normalize(velocity);
        friction *= glm::clamp(frictionAmount * deltaTime, 0.f, speed);

        velocity += friction;
        velocity = ClampMagnitude(velocity, settings.minSpeed, settings.maxSpeed);

        bodyRotation += glm::clamp(settings.bodyRotSpeed * deltaTime, 0.f, glm::abs(bodyRotation - velRotation)) * glm::sign(velRotation - bodyRotation);

        prevRotation = velRotation;
        transform->SetLocalRotation(glm::vec3(0, glm::radians(bodyRotation), 0));
    } else velocity = glm::vec3(0, 0, 0);


    float carBodyZCurAngle = carBody->transform->GetLocalRotation().y;
    carBody->transform->SetLocalRotation(glm::vec3(0, glm::mix(carBodyZCurAngle, carBodyZAngle, 5.f * deltaTime), 0));

    transform->SetWorldPosition(transform->GetWorldPosition() + velocity);

    for(auto wheel : wheels)
    {
        wheel->transform->SetLocalRotation(glm::vec3(0, 0, -transform->GetLocalRotation().y + glm::radians(velRotation)));
    }
}
