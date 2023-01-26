#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Transform
{
    public:
        Transform();

        void SetParent(Transform* parent);
        glm::mat4 GetModelMatrix() const;

        glm::vec3 GetLocalPosition() const;
        void SetLocalPosition(glm::vec3 position);

        glm::vec3 GetWorldPosition() const;
        void SetWorldPosition(glm::vec3 position);
        void SetWorldPosition(float x, float y, float z);

        glm::vec3 GetLocalRotation() const;
        void SetLocalRotation(glm::vec3 rotation);

        glm::vec3 GetWorldRotation() const;
        void SetWorldRotation(glm::vec3 rotation);

        glm::vec3 GetLocalScale() const;
        void SetLocalScale(glm::vec3 scale);

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 localScale;
        glm::mat4 model;

        Transform* parent;
        std::vector<std::weak_ptr<Transform>> children;
};

#endif
