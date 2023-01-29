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

void Transform::SetParent(Transform* parent)
{
    // TODO:keep world position when changing parent
    this->parent = parent;
}

// TODO: save this on update, we dont want it to recurse over the whole heirarchy again and again
glm::mat4 Transform::GetModelMatrix() const 
{
    if(parent == nullptr) return this->model;
    return parent->GetModelMatrix() * this->model;
}

glm::vec3 Transform::GetLocalPosition() const
{
    return position;
}

void Transform::SetLocalPosition(glm::vec3 position)
{
    this->position = position;
    
    glm::mat4 model(1);
    model = glm::translate(model, position);

    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

    model = glm::scale(model, localScale);

    // model = glm::translate(model, position);
    this->model = model;
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

glm::vec3 Transform::GetWorldRotation() const
{
    if(parent == nullptr) return rotation;
    return parent->model * glm::vec4(rotation, 1);
}

void Transform::SetWorldRotation(glm::vec3 rotation)
{
    if(parent == nullptr)
    {
        SetLocalRotation(rotation);
        return;
    }
    glm::mat4 inv = glm::inverse(parent->model);
    SetLocalRotation(inv * glm::vec4(rotation, 1));
}

glm::vec3 Transform::GetLocalScale() const
{
    return localScale;
}

void Transform::SetLocalScale(glm::vec3 scale)
{
    localScale = scale;
}
