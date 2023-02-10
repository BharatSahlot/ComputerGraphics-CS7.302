#ifndef CAMERA_H
#define CAMERA_H

#include "Engine/Object.hpp"
#include <glm/glm.hpp>

// TODO: add transform component to camera
class Camera : public Object
{
    public:
        using Object::Object;

        void SetPerspective(float fov, float aspect);

        // match height orthographic projection
        void SetOrthographic(float width, float height);

        void Start() override;
        void Tick(float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override {}

        glm::mat4 ViewProj() const;
        glm::mat4 Proj() const { return proj; }
        glm::mat4 View() const { return view; }

    private:
        glm::vec3 pos;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec2 rot;

        glm::mat4 view;
        glm::mat4 proj;
};

#endif
