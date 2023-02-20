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
    ModelToHeirarchy(model);
    oneMinusAlpha = 0.f;
    color = glm::vec3(1, 1, 1);
}

Object::Object(World* world, std::string name, const std::string& modelName)
{
    this->world = world;
    this->name = name;
    std::shared_ptr<Model> model = world->GetResourceManager().Get<Model>(modelName);
    ModelToHeirarchy(model);
    oneMinusAlpha = 0.f;
    color = glm::vec3(1, 1, 1);
}

void Object::ModelToHeirarchy(std::shared_ptr<Model> model)
{
    transform = new Transform(model->GetTransform());
    meshes = model->GetMeshes();

    modelBounds = model->GetModelBounds(*model);

    isTransparent = false;
    for(const auto& x: meshes)
    {
        if(x->GetMaterial().IsTransparent()) isTransparent = true;
    }

    std::vector<std::shared_ptr<Model>> chd = model->GetChildren();
    for(auto x: chd)
    {
        std::shared_ptr<Object> obj = world->Instantiate<Object>(x->GetName(), x);
        obj->SetParent(*this);
        children.push_back(obj);
    }
}

void Object::SetParent(const Object& obj)
{
    transform->SetParent(obj.transform);
}

Bounds Object::GetBounds() const
{
    return modelBounds;
}

void Object::Render(const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!isActive) return;

    glm::mat4 model = transform->GetModelMatrix();
    for(const auto& x: meshes)
    {
        x->Render(model, viewMat, projMat, oneMinusAlpha, color);
    }
}

void Object::Render(const glm::mat4& pModel, const glm::mat4& viewMat, const glm::mat4& projMat)
{
    if(!isActive) return;

    glm::mat4 model = pModel * transform->GetLocalModelMatrix();
    for(const auto& x: meshes)
    {
        x->Render(model, viewMat, projMat, oneMinusAlpha, color);
    }
}

void Object::SetActive(bool active)
{
    isActive = active;
    for(auto obj : children)
    {
        obj->SetActive(active);
    }
}
