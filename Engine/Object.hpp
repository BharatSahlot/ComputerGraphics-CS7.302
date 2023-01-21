#ifndef OBJECT_H
#define OBJECT_H

#include "Engine/Components/Transform.hpp"
#include "Engine/Render/Mesh.hpp"

#include <memory>

class Object
{
    public:
        Transform* transform;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;

        Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

        void Render(const glm::mat4& viewMat, const glm::mat4& projMat);
};

#endif
