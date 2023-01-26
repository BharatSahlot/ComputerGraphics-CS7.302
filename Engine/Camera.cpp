#include "Camera.hpp"
#include "glm/fwd.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Camera::SetPerspective(float fov, float aspect)
{
    this->proj = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
}

// match height orthographic projection
void Camera::SetOrthographic(float width, float height)
{
    float aspect = width / height;
    float hh = 10;
    float hw = aspect * hh;
    this->proj = glm::ortho(0.f, hw, 0.f, hh);
}

glm::mat4 Camera::ViewProj() const
{
    return this->proj * this->view;
}
