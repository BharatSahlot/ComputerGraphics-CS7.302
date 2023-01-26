#include "Object.hpp"
#include "Engine/Utils/Util.hpp"
#include <iostream>
#include <string>

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
    this->mesh = mesh;
    this->material = material;

    this->transform = new Transform();
}

void Object::UseTexture(std::shared_ptr<Texture> tex)
{
    this->textures.push_back(tex);
}

void Object::Render(const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!this->material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    for(size_t i = 0; i < this->textures.size(); i++)
    {
        this->textures[i]->Use(i);
    }

    this->material->Use();

    for(size_t i = 0; i < this->textures.size(); i++)
    {
        std::string tex = "texture";
        tex.append(std::to_string(i + 1));
        this->material->SetInt(tex, i);
    }

    this->material->SetUniformMat4("view", viewMat);
    this->material->SetUniformMat4("proj", projMat);
    this->material->SetUniformMat4("model", this->transform->GetModelMatrix());

    this->mesh->Render();
}
