#ifndef OBJECT_H
#define OBJECT_H

#include "Engine/Components/Transform.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Render/Model.hpp"
#include "Engine/Render/Texture.hpp"

#include <memory>
#include <vector>

class World;

class Object
{
    public:
        World* world;
        std::string name;
        bool isActive = true;

        Transform* transform;

        Object(std::string name) : name(name), transform(new Transform) {}
        Object(World* world, std::string name) : world(world), name(name), transform(new Transform) {}
        Object(World* world, std::string name, std::shared_ptr<Model> model);
        Object(World* world, std::string name, const std::string& model);

        void SetParent(const Object& obj);
        bool IsTransparent() const { return isTransparent; }

        Bounds GetBounds() const;

        virtual void Render(const glm::mat4& viewMat, const glm::mat4& projMat);
        virtual void Render(const glm::mat4& model, const glm::mat4& viewMat, const glm::mat4& projMat);

        virtual void Start() {}
        virtual void Tick(float deltaTime) {}

        const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return meshes; }
        const std::vector<std::shared_ptr<Object>>& GetChildren() const { return children; }

    protected:
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Object>> children;

    private:
        Bounds modelBounds;
        bool isTransparent;
        void ModelToHeirarchy(std::shared_ptr<Model> model);
};

#endif
