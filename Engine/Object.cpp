#include "Object.hpp"
#include <iostream>

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
    this->mesh = mesh;
    this->material = material;

    this->transform = new Transform();
}

void Object::Render(const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!this->material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    this->material->Use();

    this->material->SetUniformMat4("view", viewMat);
    this->material->SetUniformMat4("proj", projMat);
    this->material->SetUniformMat4("model", this->transform->GetModelMatrix());

    this->mesh->Render();
}
