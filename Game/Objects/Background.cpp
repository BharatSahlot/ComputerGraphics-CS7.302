#include "Background.hpp"
#include "Engine/Object.hpp"
#include "Globals.hpp"
#include "glm/fwd.hpp"

Background::Background(const std::string& file, float speed, float z) : Object(Plane, BasicTexMat)
{
    auto tex = std::shared_ptr<Texture>(Texture::MakeTexture(file));
    this->UseTexture(tex);

    this->transform->SetLocalScale(glm::vec3(18.46, 10, 1));
    this->transform->SetWorldPosition(glm::vec3(18.46 / 2, 5, z));

    this->speed = speed;
}

void Background::Tick(const Window &window, float deltaTime)
{
    glm::vec3 pos = this->transform->GetWorldPosition();
    pos.x += this->speed * deltaTime;

    // dont hardcode
    if(pos.x < -9.23) pos.x = 9.23;

    this->transform->SetWorldPosition(pos);
}

void Background::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    Object::Render(viewMat, projMat);
    this->transform->SetWorldPosition(this->transform->GetWorldPosition() + glm::vec3(18.46, 0, 0));
    Object::Render(viewMat, projMat);
    this->transform->SetWorldPosition(this->transform->GetWorldPosition() - glm::vec3(18.46, 0, 0));
}
