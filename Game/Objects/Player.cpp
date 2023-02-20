#include "Player.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/intersect.hpp"

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

bool TriangleLineIntersection(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, glm::vec3 l1, glm::vec3 l2, glm::vec3& p)
{
    // return glm::intersectLineTriangle(l1, glm::normalize(l2 - l1), t1, t2, t3, p);
    glm::vec3 dir = glm::normalize(l2 - l1);
    glm::vec3 norm = glm::normalize(glm::cross(
                    glm::normalize(t2 - t1),
                    glm::normalize(t3 - t1)
                ));

    float d = glm::dot(norm, t1);
    float na = glm::dot(norm, l1);
    float nb = glm::dot(norm, dir);

    p = l1 + (((d - na) / nb) * dir);

    // compute barycentric coordiantes of p
    glm::vec3 u = t2 - t1;
    glm::vec3 v = t3 - t1;
    glm::vec3 n = glm::cross(u, v);
    glm::vec3 w = p - t1;
    float beta = glm::dot(glm::cross(w, v), n) / glm::dot(n, n);
    float gamma = glm::dot(glm::cross(u, w), n) / glm::dot(n, n);
    float alpha = 1 - gamma - beta;

    // point not in triangle
    if(alpha < 0 || alpha > 1 || beta < 0 || beta > 1 || gamma < 0 || gamma > 1) return false;

    float d2 = distance2(l1, l2);
    return distance2(l2, p) <= d2 && distance2(l1, p) <= d2;
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

    // boundary = world->GetObjectByName<Object>("Collider");
    boundaryColliders = world->GetObjectsByPrefix<Object>("Collider");

    // this is very big for some reason, thats why divide by 3
    collisionRadius = glm::length(GetBounds().extents) / 3.f;

    // checkpoints
    checkpoints = world->GetObjectsByPrefix<Object>("Checkpoint");
    for(auto x: checkpoints) x->SetActive(false);
    std::sort(checkpoints.begin(), checkpoints.end(), [](Object* a, Object* b) {
        return a->name < b->name;
    });
    checkpoints[1]->SetActive(true);

    fuelCans = world->GetObjectsByPrefix<Object>("FuelCan");

    timer.Start();
    checkpointsCleared = lapsDone = 0;
    fuel = GetMaxFuel();
    health = 100.f;
    Respawn();
}

void Player::Tick(float deltaTime)
{
    if(collided)
    {
        if(collisionTimer.TimeSinceStart() >= 2.f)
        {
            collided = false;
            auto mat = world->GetResourceManager().Get<Material>("Shaders/base.fs");
            mat->Use();
            mat->SetFloat("oneMinusAlpha", 0);
        } else
        {
            auto mat = world->GetResourceManager().Get<Material>("Shaders/base.fs");
            mat->Use();
            float t = glm::pow(glm::sin(timer.TimeSinceStart() * 5.f), 4.f);
            t = glm::round(t);
            mat->SetFloat("oneMinusAlpha", t);
        }
    }

    float delta = 0.f;
    if(fuel > 0.f && world->GetWindow().GetKeyDown(GLFW_KEY_W)) delta = settings.accel;
    if(fuel > 0.f && world->GetWindow().GetKeyDown(GLFW_KEY_S)) delta -= settings.brake;

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

    if(CheckWallCollision())
    {
        collided = true;
        collisionTimer.Start();
        health -= 30.f;
        Respawn();
    }

    if(CheckCheckpointCollision())
    {
        if(onCheckPointReached) onCheckPointReached(checkpointsCleared + 1);

        checkpointsCleared++;
        if(checkpointsCleared >= checkpoints.size())
        {
            lapsDone++;
            checkpointsCleared = 0;
        }
        checkpoints[(checkpointsCleared + 1) % checkpoints.size()]->SetActive(true);
    }

    if(delta != 0) fuel -= deltaTime * 2.f;

    Object* can = CheckFuelcanCollision();
    if(can != nullptr)
    {
        fuel += GetMaxFuel() / 3.f;
        if(fuel >= GetMaxFuel()) fuel = GetMaxFuel();
        if(onFuelcanCollision) onFuelcanCollision(can);
    }
}

void Player::Respawn()
{
    // first reset all variables
    velocity = glm::vec3(0, 0, 0);
    velRotation = bodyRotation = prevRotation = 0.f;

    glm::vec3 position = checkpoints[checkpointsCleared]->transform->GetWorldPosition();
    Object* cpoint = checkpoints[checkpointsCleared];

    transform->SetWorldPosition(position * glm::vec3(1, 0, 1)); // remove the y component

    const std::vector<float>& vertices = cpoint->GetMeshes()[0]->GetVertices();
    const std::vector<unsigned int>& faces = cpoint->GetMeshes()[0]->GetFaces();

    glm::mat4 mat = cpoint->transform->GetModelMatrix();
    glm::vec3 a = mat * glm::vec4(vertices[8 * faces[0]], 
            vertices[8 * faces[0] + 1],
            vertices[8 * faces[0] + 2], 1);

    glm::vec3 b = mat * glm::vec4(vertices[8 * faces[1]],
            vertices[8 * faces[1] + 1],
            vertices[8 * faces[1] + 2], 1);

    glm::vec3 c = mat * glm::vec4(vertices[8 * faces[2]],
            vertices[8 * faces[2] + 1],
            vertices[8 * faces[2] + 2], 1);

    glm::vec3 norm = glm::normalize(glm::cross(
                    glm::normalize(b - a),
                    glm::normalize(c - a)
                ));

    float angle = glm::orientedAngle(glm::vec3(0, 0, -1), norm, glm::vec3(0, 1, 0));
    float angleD = glm::degrees(angle);

    velRotation = bodyRotation = prevRotation = angleD;
    transform->SetLocalRotation(glm::vec3(0, angle, 0));
}

bool Player::CheckWallCollision()
{
    std::vector<glm::vec3> points = GetBounds().GetRotatedMeanPlane(transform->GetModelMatrix());
    for(auto collider: boundaryColliders)
    {
        float dist = glm::distance(collider->transform->GetWorldPosition(), transform->GetWorldPosition());
        if(dist <= collisionRadius)
        {
            const auto& meshes = collider->GetMeshes();
            glm::mat4 mat = collider->transform->GetModelMatrix();
            for(auto mesh: meshes)
            {
                const std::vector<float>& vertices = mesh->GetVertices();
                const std::vector<unsigned int>& faces = mesh->GetFaces();

                for(size_t i = 0; i < faces.size(); i += 3)
                {
                    glm::vec3 a = mat * glm::vec4(vertices[8 * faces[i]], 
                                            vertices[8 * faces[i] + 1],
                                            vertices[8 * faces[i] + 2], 1);

                    glm::vec3 b = mat * glm::vec4(vertices[8 * faces[i + 1]],
                                            vertices[8 * faces[i + 1] + 1],
                                            vertices[8 * faces[i + 1] + 2], 1);

                    glm::vec3 c = mat * glm::vec4(vertices[8 * faces[i + 2]],
                                            vertices[8 * faces[i + 2] + 1],
                                            vertices[8 * faces[i + 2] + 2], 1);

                    std::vector<int> lines({
                        0, 1,
                        1, 2,
                        2, 3,
                        3, 0,
                    });

                    world->DrawLine(a, b);
                    world->DrawLine(b, c);
                    world->DrawLine(a, c);
                    for(int l = 0; l < lines.size() / 2; l++)
                    {
                        glm::vec3 p1 = points[lines[2 * l]];
                        glm::vec3 p2 = points[lines[2 * l + 1]];

                        glm::vec3 dir = glm::normalize(p1 - p2);
                        glm::vec3 dir2 = glm::normalize(p2 - p1);

                        glm::vec3 tb;
                        world->DrawBox(p1, glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
                        if(TriangleLineIntersection(a, b, c, p1, p2, tb))
                        {
                            world->DrawLine(p1, tb, glm::vec3(1, 0, 0));
                            world->DrawLine(p2, p1, glm::vec3(1, 0, 1));
                            return true;
                        } else if(TriangleLineIntersection(a, b, c, p2, p1, tb))
                        {
                            world->DrawLine(p2, tb, glm::vec3(0, 1, 0));
                            world->DrawLine(p2, p1, glm::vec3(0, 0, 1));
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// only checks collision with the next checkpoint
bool Player::CheckCheckpointCollision()
{
    std::vector<glm::vec3> points = GetBounds().GetRotatedMeanPlane(transform->GetModelMatrix());

    Object* checkpoint = checkpoints[(checkpointsCleared + 1) % checkpoints.size()];

    const std::vector<float>& vertices = checkpoint->GetMeshes()[0]->GetVertices();
    const std::vector<unsigned int>& faces = checkpoint->GetMeshes()[0]->GetFaces();

    glm::mat4 mat = checkpoint->transform->GetModelMatrix();
    for(size_t i = 0; i < faces.size(); i += 3)
    {
        glm::vec3 a = mat * glm::vec4(vertices[8 * faces[i]], 
                                vertices[8 * faces[i] + 1],
                                vertices[8 * faces[i] + 2], 1);

        glm::vec3 b = mat * glm::vec4(vertices[8 * faces[i + 1]],
                                vertices[8 * faces[i + 1] + 1],
                                vertices[8 * faces[i + 1] + 2], 1);

        glm::vec3 c = mat * glm::vec4(vertices[8 * faces[i + 2]],
                                vertices[8 * faces[i + 2] + 1],
                                vertices[8 * faces[i + 2] + 2], 1);

        std::vector<int> lines({
            0, 1,
            1, 2,
            2, 3,
            3, 0,
        });

        world->DrawLine(a, b);
        world->DrawLine(b, c);
        world->DrawLine(a, c);
        for(int l = 0; l < lines.size() / 2; l++)
        {
            glm::vec3 p1 = points[lines[2 * l]];
            glm::vec3 p2 = points[lines[2 * l + 1]];

            glm::vec3 dir = glm::normalize(p1 - p2);
            glm::vec3 dir2 = glm::normalize(p2 - p1);

            glm::vec3 tb;
            // world->DrawBox(p1, glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
            if(TriangleLineIntersection(a, b, c, p1, p2, tb))
            {
                world->DrawLine(p1, tb, glm::vec3(1, 0, 0));
                world->DrawLine(p2, p1, glm::vec3(1, 0, 1));
                return true;
            } else if(TriangleLineIntersection(a, b, c, p2, p1, tb))
            {
                world->DrawLine(p2, tb, glm::vec3(0, 1, 0));
                world->DrawLine(p2, p1, glm::vec3(0, 0, 1));
                return true;
            }
        }
    }
    return false;
}

Object* Player::CheckFuelcanCollision()
{
    std::vector<glm::vec3> points = GetBounds().GetRotatedMeanPlane(transform->GetModelMatrix());

    for(Object* can: fuelCans)
    {
        can = can->GetChildren()[0].get();
        auto lines = can->GetBounds().GetRotatedBox(can->transform->GetModelMatrix());
        // world->DrawRotatedBox(lines);
        if(!can->IsActive()) continue;

        float dist = glm::distance2(can->transform->GetWorldPosition(), transform->GetWorldPosition());
        if(dist > collisionRadius * collisionRadius) continue;

        const auto& meshes = can->GetMeshes();

        glm::mat4 mat = can->transform->GetModelMatrix();
        for(auto mesh: meshes)
        {
            const std::vector<float>& vertices = mesh->GetVertices();
            const std::vector<unsigned int>& faces = mesh->GetFaces();

            for(size_t i = 0; i < faces.size(); i += 3)
            {
                glm::vec3 a = mat * glm::vec4(vertices[8 * faces[i]], 
                        vertices[8 * faces[i] + 1],
                        vertices[8 * faces[i] + 2], 1);

                glm::vec3 b = mat * glm::vec4(vertices[8 * faces[i + 1]],
                        vertices[8 * faces[i + 1] + 1],
                        vertices[8 * faces[i + 1] + 2], 1);

                glm::vec3 c = mat * glm::vec4(vertices[8 * faces[i + 2]],
                        vertices[8 * faces[i + 2] + 1],
                        vertices[8 * faces[i + 2] + 2], 1);

                std::vector<int> lines({
                        0, 1,
                        1, 2,
                        2, 3,
                        3, 0,
                        });

                // world->DrawLine(a, b);
                // world->DrawLine(b, c);
                // world->DrawLine(a, c);
                for(int l = 0; l < lines.size() / 2; l++)
                {
                    glm::vec3 p1 = points[lines[2 * l]];
                    glm::vec3 p2 = points[lines[2 * l + 1]];

                    glm::vec3 dir = glm::normalize(p1 - p2);
                    glm::vec3 dir2 = glm::normalize(p2 - p1);

                    glm::vec3 tb;
                    // world->DrawBox(p1, glm::vec3(1, 1, 1), glm::vec3(1, 0, 0));
                    if(TriangleLineIntersection(a, b, c, p1, p2, tb))
                    {
                        // world->DrawLine(p1, tb, glm::vec3(1, 0, 0));
                        // world->DrawLine(p2, p1, glm::vec3(1, 0, 1));
                        return can;
                    } else if(TriangleLineIntersection(a, b, c, p2, p1, tb))
                    {
                        // world->DrawLine(p2, tb, glm::vec3(0, 1, 0));
                        // world->DrawLine(p2, p1, glm::vec3(0, 0, 1));
                        return can;
                    }
                }
            }
        }
    }
    return nullptr;
}
