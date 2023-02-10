#include "Object.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Util.hpp"
#include "Engine/World.hpp"
#include <iostream>
#include <string>
#include <vector>

Object::Object(World* world, std::string name, std::shared_ptr<Model> model)
{
    this->world = world;
    this->name = name;
    this->model = model;
    this->transform = new Transform();
}

Object::Object(World* world, std::string name, const std::string& model)
{
    this->world = world;
    this->name = name;
    this->model = world->GetResourceManager().Get<Model>(model);
    this->transform = new Transform();
}

void Object::Render(const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!model) return;
    model->Render(viewMat, projMat);
}
