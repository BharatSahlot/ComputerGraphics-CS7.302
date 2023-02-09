#ifndef WORLD_H
#define WORLD_H

#include "Engine/Resource/ResourceManager.hpp"
#include <memory>

class Window;

class World
{
    public:
        World(std::shared_ptr<Window> window);

        ResourceManager& GetResourceManager() const { return *resourceManager; }
        Window& GetWindow() const { return *window; }

    private:
        std::unique_ptr<ResourceManager> resourceManager;
        std::shared_ptr<Window> window;
};

#endif
