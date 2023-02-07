#ifndef OBJECT_H
#define OBJECT_H

#include "Engine/Components/Transform.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Window/Window.hpp"

#include <memory>
#include <vector>

class Object
{
    public:
        std::string name;

        Transform* transform;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;

        Object(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

        void UseTexture(std::shared_ptr<Texture> tex);

        virtual void Render(const glm::mat4& viewMat, const glm::mat4& projMat);

        virtual void Start() {}
        virtual void Tick(const Window& window, float deltaTime) {}

    protected:
        std::vector<std::shared_ptr<Texture>> textures;
};

#endif
