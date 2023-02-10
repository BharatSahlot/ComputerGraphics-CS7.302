#include "Camera.hpp"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "World.hpp"
#include "Engine/Window/Window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Camera::SetPerspective(float fov, float aspect)
{
    proj = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
}

// match height orthographic projection
void Camera::SetOrthographic(float width, float height)
{
    float aspect = width / height;
    float hh = 10;
    float hw = aspect * hh;
    proj = glm::ortho(0.f, hw, 0.f, hh);
}

void Camera::Start()
{
    pos = glm::vec3(0, 0, -35);
    up = glm::vec3(0, 1, 0);
    front = glm::vec3(0, 0, 1);
    rot = glm::vec2(0, -90);
}

void Camera::Tick(float deltaTime)
{
    auto delta = world->GetWindow().GetCursorDelta() * 0.2f;

    rot.x += delta.y;
    rot.y += delta.x;

    rot.x = glm::clamp(rot.x, -89.f, 89.f);

    front.x = std::cos(glm::radians(rot.y)) * std::cos(glm::radians(rot.x));
    front.y = std::sin(glm::radians(rot.x));
    front.z = -std::sin(glm::radians(rot.y)) * std::cos(glm::radians(rot.x));
    front = glm::normalize(front);

    std::cout << front.x << ' ' << front.y << ' ' << front.z << std::endl;
    // std::cout << rot.x << ' ' << rot.y << std::endl;

    right   = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    up      = glm::normalize(glm::cross(right, front));

    glm::vec3 offset(0);
    if(world->GetWindow().GetKeyDown(GLFW_KEY_W)) offset += front;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_S)) offset -= front;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_A)) offset -= right;
    if(world->GetWindow().GetKeyDown(GLFW_KEY_D)) offset += right;
    // std::cout << offset.x << ' ' << offset.y << ' ' << offset.z << std::endl;
    // offset = normalize(offset);

    offset *= 10 * deltaTime;
    pos += offset;

    view = glm::lookAt(pos, pos + front, up);
    // view = glm::lookAt(pos, glm::vec3(0), glm::vec3(0, 1, 0));

    // transform->SetWorldRotation(transform->GetWorldRotation() + offset);
}

glm::mat4 Camera::ViewProj() const
{
    return Proj() * View();
}
