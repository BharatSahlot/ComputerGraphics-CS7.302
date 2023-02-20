#ifndef CAMERA_H
#define CAMERA_H

#include "Engine/Object.hpp"
#include <glm/glm.hpp>

class Camera : public Object
{
    public:
        using Object::Object;
        glm::vec3 clearColor;
        
        bool canMove = true;

        Camera(World* world, std::string name, glm::vec3 pos, glm::vec3 look);

        void SetPerspective(float fov, float aspect);

        // match height orthographic projection
        void SetOrthographic(float width, float height);

        void Start() override;
        void Tick(float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override {}

        void Use(glm::vec2 windowDims, glm::vec3 viewport = glm::vec3(0, 0, 0.1)) const;

        glm::mat4 ViewProj() const;
        glm::mat4 Proj() const { return proj; }
        glm::mat4 View() const { return view; }

        glm::vec3 Position() const { return pos; }

    private:
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec2 rot;

    protected:
        glm::vec3 pos;
        glm::mat4 view;
        glm::mat4 proj;
};

#endif
