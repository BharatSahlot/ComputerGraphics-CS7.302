#include "AIPlayer.hpp"
#include "Engine/Utils/Random.hpp"
#include "Game/Objects/Player.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

extern glm::vec3 CalcForward(glm::vec3 rotation);

extern glm::vec3 ClampMagnitude(glm::vec3 vec, float minMag, float maxMag);

extern bool TriangleLineIntersection(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, glm::vec3 l1, glm::vec3 l2, glm::vec3& p);
extern float AbsMin(float a, float b);

extern void SetAlpha(Object* obj, float alpha);

void SetColor(Object* obj, glm::vec3 color) 
{
    obj->color = color;
    for(auto ch: obj->GetChildren()) SetColor(ch.get(), color);
}

AIPlayer::AIPlayer(World* world, std::string name, std::string model, Game* game, PlayerSettings settings, glm::vec3 spawnOffset)
    : Object(world, name, model), game(game), settings(settings), spawnOffset(spawnOffset)
{
    color = glm::vec3(Random::GetFloat(), Random::GetFloat(), Random::GetFloat());
}

glm::vec3 AIPlayer::GetPointAlongCheckpoint(int i)
{
    glm::vec3 pos = targetLines[i].first + (1.f - 2.f * Random::GetFloat()) * 50.f * targetLines[i].second;
    return pos;
}

void AIPlayer::Start()
{
    SetColor(this, color);
    velocity = glm::vec3(0, 0, 0);
    velRotation = 0;

    // carBody = world->GetObjectByName<Object>("Body");
    carBody = GetChildByName("Body");

    checkpoints = world->GetObjectsByPrefix<Object>("Checkpoint");
    std::sort(checkpoints.begin(), checkpoints.end(), [](Object* a, Object* b) {
        return a->name < b->name;
    });

    for(int i = 0; i < checkpoints.size(); i++)
    {
        auto cpoint = checkpoints[i];
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

        glm::vec3 normnorm = glm::normalize(glm::cross(norm, glm::vec3(0, 1, 0)));
        targetLines.push_back({ cpoint->transform->GetWorldPosition() * glm::vec3(1, 0, 1), normnorm });

        glm::vec3 pos = targetLines[i].first + spawnOffset;
    }
    transform->SetWorldPosition(targetLines[0].first + spawnOffset);

    // boundary = world->GetObjectByName<Object>("Collider");
    boundaryColliders = world->GetObjectsByPrefix<Object>("Collider");

    // this is very big for some reason, thats why divide by 3
    collisionRadius = glm::length(GetBounds().extents) / 3.f;

    timer.Start();
    checkpointsCleared = lapsDone = 0;

    targetPos = GetPointAlongCheckpoint(1);
}

void AIPlayer::Tick(float deltaTime)
{
    if(collided)
    {
        if(collisionTimer.TimeSinceStart() >= 2.f)
        {
            collided = false;
            SetAlpha(this, 0);
        } else
        {
            float t = glm::pow(glm::sin(timer.TimeSinceStart() * 5.f), 4.f);
            t = glm::round(t);
            SetAlpha(this, t);
        }
    }

    glm::vec3 dir = glm::normalize(targetPos - transform->GetWorldPosition());
    float targetRotation = glm::orientedAngle(glm::vec3(0, 0, 1), dir, glm::vec3(0, 1, 0));
    targetRotation = glm::degrees(targetRotation);

    if(velRotation < targetRotation)
    {
        velRotation += settings.rotSpeed * deltaTime;
        if(velRotation > targetRotation) velRotation = targetRotation;
    } else if(velRotation > targetRotation)
    {
        velRotation -= settings.rotSpeed * deltaTime;
        if(velRotation < targetRotation) velRotation = targetRotation;
    }

    glm::vec3 forward = CalcForward(glm::vec3(0, glm::radians(velRotation), 0));
    velocity += forward * settings.accel * deltaTime;

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

        transform->SetLocalRotation(glm::vec3(0, glm::radians(velRotation), 0));
    } else velocity = glm::vec3(0, 0, 0);

    transform->SetWorldPosition(transform->GetWorldPosition() + velocity);

    float carBodyZCurAngle = carBody->transform->GetLocalRotation().y;
    carBody->transform->SetLocalRotation(glm::vec3(0, glm::mix(carBodyZCurAngle, carBodyZAngle, 5.f * deltaTime), 0));

    if(CheckWallCollision())
    {
        collided = true;
        collisionTimer.Start();
        targetPos = GetPointAlongCheckpoint((checkpointsCleared + 1) % checkpoints.size());
        Respawn();
    }

    if(CheckCheckpointCollision())
    {
        checkpointsCleared++;
        if(checkpointsCleared >= checkpoints.size())
        {
            lapsDone++;
            checkpointsCleared = 0;
        }
        int c = (checkpointsCleared + 1) % checkpoints.size();
        targetPos = GetPointAlongCheckpoint(c);
    }
}

void AIPlayer::Respawn()
{
    // std::cout << name << std::endl;
    // first reset all variables
    velocity = glm::vec3(0, 0, 0);
    velRotation = 0.f;

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

    velRotation = angleD;
    transform->SetLocalRotation(glm::vec3(0, angle, 0));
}

bool AIPlayer::CheckWallCollision()
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
                            return true;
                        } else if(TriangleLineIntersection(a, b, c, p2, p1, tb))
                        {
                            // world->DrawLine(p2, tb, glm::vec3(0, 1, 0));
                            // world->DrawLine(p2, p1, glm::vec3(0, 0, 1));
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
bool AIPlayer::CheckCheckpointCollision()
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
                return true;
            } else if(TriangleLineIntersection(a, b, c, p2, p1, tb))
            {
                // world->DrawLine(p2, tb, glm::vec3(0, 1, 0));
                // world->DrawLine(p2, p1, glm::vec3(0, 0, 1));
                return true;
            }
        }
    }
    return false;
}
