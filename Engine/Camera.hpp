#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
    public:
        glm::mat4 viewMat;
        void SetPerspective(float fov, float aspect);
        const glm::mat4& ViewProj() const;

    private:
        glm::mat4 proj;
        glm::mat4 vp;
};

#endif
