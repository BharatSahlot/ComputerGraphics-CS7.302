#include "Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Camera::SetPerspective(float fov, float aspect)
{
    this->proj = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
}

glm::mat4 Camera::ViewProj() const
{
    return this->proj * this->view;
}
