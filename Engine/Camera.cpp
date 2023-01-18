#include "Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Camera::SetPerspective(float fov, float aspect)
{
    this->proj = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
    this->vp = this->proj * this->viewMat;
}

const glm::mat4& Camera::ViewProj() const
{
    return this->vp;
}
