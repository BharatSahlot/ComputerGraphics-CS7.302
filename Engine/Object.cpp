#include "Object.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include <iostream>
#include <string>
#include <vector>

Object::Object(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
    this->name = name;
    this->mesh = mesh;
    this->material = material;

    this->transform = new Transform();
}

void Object::UseTexture(std::shared_ptr<Texture> tex)
{
    textures.push_back(tex);
}

void Object::Render(const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!material)
    {
        std::cerr << "No material assigned to object" << std::endl;
        return;
    }

    for(size_t i = 0; i < textures.size(); i++)
    {
        textures[i]->Use(i);
    }

    material->Use();

    for(size_t i = 0; i < this->textures.size(); i++)
    {
        std::string tex = "texture";
        tex.append(std::to_string(i + 1));
        material->SetInt(tex, i);
    }

    material->SetUniformMat4("view", viewMat);
    material->SetUniformMat4("proj", projMat);
    material->SetUniformMat4("model", this->transform->GetModelMatrix());

    mesh->Render();
}
