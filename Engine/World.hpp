#ifndef WORLD_H
#define WORLD_H

#include "Engine/Object.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include <memory>

class Window;

class World
{
    public:
        World(std::shared_ptr<Window> window);

        ResourceManager& GetResourceManager() const { return *resourceManager; }
        Window& GetWindow() const { return *window; }

        void Tick(float deltaTime) const;
        void Render() const;

        template<typename T, class... Us>
        std::shared_ptr<T> Instantiate(std::string name, Us... args)
        {
            std::shared_ptr<T> ptr(new T(this, name, args...));
            objects.push_back(ptr);
            return ptr;
        }

    private:
        std::unique_ptr<ResourceManager> resourceManager;
        std::shared_ptr<Window> window;

        std::vector<std::shared_ptr<Object>> objects;
};

#endif
