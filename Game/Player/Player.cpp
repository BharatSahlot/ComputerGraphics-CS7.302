#include "Player.hpp"
#include "Engine/Object.hpp"

#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/Window/Window.hpp"
#include "Globals.hpp"
#include "glm/common.hpp"
#include <iostream>

Player::Player() : Object(Plane, BasicMat)
{
    this->idleSheet = Texture::MakeTexture("Game/Assets/Player_Idle.png", GL_LINEAR);
    this->walkSheet = Texture::MakeTexture("Game/Assets/Player_Run.png", GL_LINEAR);
    this->flySheet = Texture::MakeTexture("Game/Assets/Player_Fly.png", GL_LINEAR);
    if(this->walkSheet == nullptr || this->flySheet == nullptr)
    {
        std::cerr << "Error loading spritesheet" << std::endl;
    }

    Shader* vs = Shader::MakeShader("Shaders/base.vs", GL_VERTEX_SHADER);
    Shader* fs = Shader::MakeShader("Shaders/player.fs", GL_FRAGMENT_SHADER);
    this->material = std::shared_ptr<Material>(Material::MakeMaterial(vs, fs));
}

void Player::Start()
{
    this->transform->SetWorldPosition(4.f, 9.f, 0.f);
    this->transform->SetLocalScale(glm::vec3(2, 1.5, 1));
    frameTimer.Start();
    this->frame = 0;
    this->currentSheet = this->idleSheet;
}

void Player::SetCurrentSheet(Texture* sheet)
{
    if(this->currentSheet != sheet)
    {
        this->currentSheet = sheet;
        this->frame = 0;
        this->frameTimer.Start();
    }
}

void Player::Tick(const Window& window, float deltaTime)
{
    this->velocity.y += GRAVITY * deltaTime;

    bool flying = false;
    if(window.GetKey(GLFW_KEY_SPACE))
    {
        this->velocity.y += NITRO * deltaTime;
        this->col = glm::vec3(2.f, 2.f, 2.f);
        this->SetCurrentSheet(this->flySheet);
        flying = true;
    } else
    {
        this->col = glm::vec3(1.f, 1.f, 1.f);
    }

    this->velocity.y = glm::clamp(this->velocity.y, -MAX_Y_VELOCITY, MAX_Y_VELOCITY);

    glm::vec3 pos = this->transform->GetWorldPosition();
    pos += this->velocity * deltaTime;

    if(pos.y >= CEILING && this->velocity.y > 0)
    {
        pos.y = CEILING;
        this->velocity.y = 0;
    }

    float playerHeight = (this->transform->GetLocalScale().y / 2) - 0.1;
    if(pos.y <= GROUND + GROUND_HEIGHT + playerHeight && this->velocity.y < 0)
    {
        pos.y = GROUND + GROUND_HEIGHT + playerHeight;
        this->velocity.y = 0;
    }

    if(!flying)
    {
        if(pos.y == GROUND + GROUND_HEIGHT + playerHeight)
        {
            this->SetCurrentSheet(this->walkSheet);
        } else
        {
            this->SetCurrentSheet(this->idleSheet);
        }
    }

    this->transform->SetWorldPosition(pos);

    int ind = (int)(frameTimer.TimeSinceStart() * 1000) / SPRITE_ANIM_TIME_PER_FRAME;
    if(ind > 0)
    {
        this->frame = (this->frame + ind) % 15;
        frameTimer.Start();
    }
}

void Player::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    if(!this->material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    this->material->Use();
    this->material->SetVec3("col", glm::vec3(0.3, 0.2, 0.8));
    this->material->SetInt("texXCount", 5);
    this->material->SetInt("texYCount", 3);
    this->material->SetInt("index", this->frame);

    this->material->SetInt("texture1", 0);
    this->currentSheet->Use(0);

    glm::vec2 texDims = currentSheet->GetDims();
    this->material->SetVec2("texDims", texDims);
    this->material->SetVec2("spriteDims", glm::vec2(texDims.x / 5.f, texDims.y / 3.f));

    this->material->SetUniformMat4("view", viewMat);
    this->material->SetUniformMat4("proj", projMat);
    this->material->SetUniformMat4("model", this->transform->GetModelMatrix());

    this->mesh->Render();
}
