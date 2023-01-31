#include "Player.hpp"
#include "Engine/Object.hpp"

#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Globals.hpp"
#include "glm/common.hpp"
#include <iostream>

Player::Player(Level* level) : Object("player", Plane, nullptr)
{
    this->level = level;

    idleSheet = Texture::MakeTexture("Game/Assets/Player_Idle.png", GL_LINEAR);
    walkSheet = Texture::MakeTexture("Game/Assets/Player_Run.png", GL_LINEAR);
    flySheet = Texture::MakeTexture("Game/Assets/Player_Fly.png", GL_LINEAR);
    if(walkSheet == nullptr || flySheet == nullptr)
    {
        std::cerr << "Error loading spritesheet" << std::endl;
    }

    Shader* vs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* fs = Shader::MakeShader("Shaders/player.fs", GL_FRAGMENT_SHADER);
    material = std::shared_ptr<Material>(Material::MakeMaterial(vs, fs));
}

void Player::Start()
{
    transform->SetWorldPosition(4.f, 9.f, 0.f);
    transform->SetLocalScale(glm::vec3(2, 1.5, 1));
    frameTimer.Start();
    frame = 0;
    currentSheet = idleSheet;
}

void Player::SetCurrentSheet(Texture* sheet)
{
    if(currentSheet != sheet)
    {
        currentSheet = sheet;
        frame = 0;
        frameTimer.Start();
    }
}

bool CheckLineIntersection(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2)
{
    float tn = (a1.x - b1.x) * (b1.y - b2.y) - (a1.y - b1.y) * (b1.x - b2.x);
    float td = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);

    if(td == 0) return false;

    float un = (a1.x - b1.x) * (a1.y - a2.y) - (a1.y - b1.y) * (a1.x - a2.x);
    float ud = td;

    if(tn < 0 && td < 0) tn *= -1, td *= -1;
    if(un < 0 && ud < 0) un *= -1, ud *= -1;

    return tn >= 0 && tn <= td && un >= 0 && un <= ud;
}

bool CheckCollision(glm::mat4 a, glm::mat4 b)
{
    std::pair<glm::vec2, glm::vec2> points[] = {
        { glm::vec2(0.2, 0.5), glm::vec2(0.2, -0.5) },
        { glm::vec2(0.2, 0.5), glm::vec2(-0.2, 0.5) },
        { glm::vec2(-0.2, -0.5), glm::vec2(-0.2, 0.5) },
        { glm::vec2(-0.2, -0.5), glm::vec2(0.2, -0.5) },
    };

    std::pair<glm::vec2, glm::vec2> points1[] = {
        { glm::vec2(0.5, 0.5), glm::vec2(0.5, -0.5) },
        { glm::vec2(0.5, 0.5), glm::vec2(-0.5, 0.5) },
        { glm::vec2(-0.5, -0.5), glm::vec2(-0.5, 0.5) },
        { glm::vec2(-0.5, -0.5), glm::vec2(0.5, -0.5) },
    };

    for(int i = 0; i < 4; i++)
    {
        glm::vec2 a1 = a * glm::vec4(points[i].first, 0, 1);
        glm::vec2 a2 = a * glm::vec4(points[i].second, 0, 1);
        for(int j = 0; j < 4; j++)
        {
            glm::vec2 b1 = b * glm::vec4(points1[j].first, 0, 1);
            glm::vec2 b2 = b * glm::vec4(points1[j].second, 0, 1);

            if(CheckLineIntersection(a1, a2, b1, b2)) return true;
        }
    }
    return false;
}

void Player::Tick(const Window& window, float deltaTime)
{
    velocity.y += GRAVITY * deltaTime;

    bool flying = false;
    if(window.GetKey(GLFW_KEY_SPACE))
    {
        velocity.y += NITRO * deltaTime;
        col = glm::vec3(2.f, 2.f, 2.f);
        SetCurrentSheet(flySheet);
        flying = true;
    } else
    {
        col = glm::vec3(1.f, 1.f, 1.f);
    }

    velocity.y = glm::clamp(velocity.y, -MAX_Y_VELOCITY, MAX_Y_VELOCITY);

    glm::vec3 pos = transform->GetWorldPosition();
    pos += velocity * deltaTime;

    if(pos.y >= CEILING && velocity.y > 0)
    {
        pos.y = CEILING;
        velocity.y = 0;
    }

    float playerHeight = (transform->GetLocalScale().y / 2) - 0.1;
    if(pos.y <= GROUND + GROUND_HEIGHT + playerHeight && velocity.y < 0)
    {
        pos.y = GROUND + GROUND_HEIGHT + playerHeight;
        velocity.y = 0;
    }

    if(!flying)
    {
        if(pos.y == GROUND + GROUND_HEIGHT + playerHeight)
        {
            SetCurrentSheet(walkSheet);
        } else
        {
            SetCurrentSheet(idleSheet);
        }
    }

    transform->SetWorldPosition(pos);

    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / SPRITE_ANIM_TIME_PER_FRAME;
    if(ind > 0)
    {
        frame = (frame + ind) % 15;
        frameTimer.Start();
    }

    static int colCount = 0;
    auto zappers = level->GetActiveZappers();
    for(auto zapper: zappers)
    {
        if(CheckCollision(transform->GetModelMatrix(), zapper->transform->GetModelMatrix()))
        {
            level->EndLevel();
        }
    }
}

void Player::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    material->Use();
    material->SetVec3("col", glm::vec3(0.3, 0.2, 0.8));
    material->SetInt("texXCount", 5);
    material->SetInt("texYCount", 3);
    material->SetInt("index", frame);

    material->SetInt("texture1", 0);
    currentSheet->Use(0);

    glm::vec2 texDims = currentSheet->GetDims();
    material->SetVec2("texDims", texDims);
    material->SetVec2("spriteDims", glm::vec2(texDims.x / 5.f, texDims.y / 3.f));

    material->SetUniformMat4("view", viewMat);
    material->SetUniformMat4("proj", projMat);
    material->SetUniformMat4("model", transform->GetModelMatrix());

    mesh->Render();
}
