#include "Transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"

Transform::Transform()
{
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    localScale = glm::vec3(1, 1, 1);
    model = glm::mat4(1);
    parent = nullptr;
}

Transform::Transform(const Transform& trans)
{
    position = trans.position;
    rotation = trans.rotation;
    localScale = trans.localScale;
    model = trans.model;
    parent = nullptr;
}

void Transform::SetParent(const Transform* parent)
{
    // TODO:keep world position when changing parent
    this->parent = parent;
}

// TODO: save this on update, we dont want it to recurse over the whole heirarchy again and again
glm::mat4 Transform::GetModelMatrix() const 
{
    if(parent == nullptr) return model;
    return parent->GetModelMatrix() * model;
}

glm::vec3 Transform::GetLocalPosition() const
{
    return position;
}

void Transform::SetLocalPosition(glm::vec3 position)
{
    this->position = position;
    
    auto translate = glm::translate(glm::mat4(1), position);

    auto rX = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
    auto rY = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
    auto rZ = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));

    model = translate * rZ * rY * rX * glm::scale(glm::mat4(1), localScale);
}

glm::vec3 Transform::GetWorldPosition() const
{
    if(parent == nullptr) return position;
    return parent->model * glm::vec4(position, 1);
}

void Transform::SetWorldPosition(glm::vec3 position)
{
    if(parent == nullptr)
    {
        SetLocalPosition(position);
        return;
    }

    glm::mat4 inv = glm::inverse(parent->model);
    SetLocalPosition(inv * glm::vec4(position, 1));
}

void Transform::SetWorldPosition(float x, float y, float z)
{
    SetWorldPosition(glm::vec3(x, y, z));
}

glm::vec3 Transform::GetLocalRotation() const
{
    return rotation;
}

void Transform::SetLocalRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    SetLocalPosition(GetLocalPosition());
}

glm::vec3 Transform::GetLocalScale() const
{
    return localScale;
}

void Transform::SetLocalScale(glm::vec3 scale)
{
    localScale = scale;
    SetLocalPosition(GetLocalPosition());
}
