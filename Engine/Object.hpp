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

        Transform* transform;

        Object(std::string name) : name(name), transform(new Transform) {}
        Object(World* world, std::string name) : world(world), name(name), transform(new Transform) {}
        Object(World* world, std::string name, std::shared_ptr<Model> model);
        Object(World* world, std::string name, const std::string& model);

        virtual void Render(const glm::mat4& viewMat, const glm::mat4& projMat);

        virtual void Start() {}
        virtual void Tick(float deltaTime) {}

    protected:
        std::shared_ptr<Model> model;
};

#endif
