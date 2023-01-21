#include "Transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"

Transform::Transform()
{
    this->position = glm::vec3(0, 0, 0);
    this->rotation = glm::vec3(0, 0, 0);
    this->model = glm::mat4(1);
    this->parent = nullptr;
}

void Transform::SetParent(Transform* parent)
{
    // TODO:keep world position when changing parent
    this->parent = parent;
}

// TODO: save this on update, we dont want it to recurse over the whole heirarchy again and again
glm::mat4 Transform::GetModelMatrix() const 
{
    if(this->parent == nullptr) return this->model;
    return this->parent->GetModelMatrix() * this->model;
}

glm::vec3 Transform::GetLocalPosition() const
{
    return this->position;
}

void Transform::SetLocalPosition(glm::vec3 position)
{
    this->position = position;
    
    glm::mat4 model(1);
    model = glm::translate(model, position);

    model = glm::rotate(model, this->rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, this->rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, this->rotation.z, glm::vec3(0, 0, 1));

    // model = glm::translate(model, position);
    this->model = model;
}

glm::vec3 Transform::GetWorldPosition() const
{
    if(this->parent == nullptr) return this->position;
    return this->parent->model * glm::vec4(this->position, 1);
}

void Transform::SetWorldPosition(glm::vec3 position)
{
    if(this->parent == nullptr)
    {
        this->SetLocalPosition(position);
        return;
    }

    glm::mat4 inv = glm::inverse(this->parent->model);
    SetLocalPosition(inv * glm::vec4(position, 1));
}

glm::vec3 Transform::GetLocalRotation() const
{
    return this->rotation;
}

void Transform::SetLocalRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    this->SetLocalPosition(this->GetLocalPosition());
}

glm::vec3 Transform::GetWorldRotation() const
{
    if(this->parent == nullptr) return this->rotation;
    return this->parent->model * glm::vec4(this->rotation, 1);
}

void Transform::SetWorldRotation(glm::vec3 rotation)
{
    if(this->parent == nullptr)
    {
        SetLocalRotation(rotation);
        return;
    }
    glm::mat4 inv = glm::inverse(this->parent->model);
    SetLocalRotation(inv * glm::vec4(rotation, 1));
}
