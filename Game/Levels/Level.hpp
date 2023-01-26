#ifndef LEVEL_H
#define LEVEL_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include <memory>
#include <vector>

class Level
{
    public:
        bool started = false;

        Level(std::shared_ptr<Camera> camera);

        virtual int Load();

        void Start();
        void Tick(float deltaTime);
        virtual int Unload();

        void Render(const glm::mat4& view, const glm::mat4& proj);

        // TODO:
        // std::weak_ptr<T> GetObjectsOfType(T type);

    protected:
        std::shared_ptr<Camera> camera;
        std::vector<std::shared_ptr<Object>> objects;
};

#endif
