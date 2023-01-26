#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

// TODO: add transform component to camera
class Camera
{
    public:
        glm::mat4 view;
        void SetPerspective(float fov, float aspect);

        // match height orthographic projection
        void SetOrthographic(float width, float height);

        glm::mat4 ViewProj() const;
        glm::mat4 Proj() const { return proj; }

    private:
        glm::mat4 proj;
};

#endif
