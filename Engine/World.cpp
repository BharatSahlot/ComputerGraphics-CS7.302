#include "World.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/Window/Window.hpp"
#include <algorithm>
#include <cmath>

World::World(std::shared_ptr<Window> window)
{
    this->window = window;
    resourceManager = std::unique_ptr<ResourceManager>(ResourceManager::CreateResourceManager(window->GetGLFWwindow(), this));
}

void World::Tick(float deltaTime) const
{
    for(auto x: objects) x->Tick(deltaTime);
}

void World::Render()
{
    float cz = window->camera->Position().z;
    std::sort(objects.begin(), objects.end(), [cz](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool {
        bool ta = a->IsTransparent();
        bool tb = b->IsTransparent();

        // a is transparent b is not
        if(ta && !tb) return false;

        // a is not b is
        if(!ta && tb) return true;

        // a and b both(not)
        float az = std::abs(cz - a->transform->GetWorldPosition().z);
        float bz = std::abs(cz - b->transform->GetWorldPosition().z);
        return az < bz;
    });
    for(auto x: objects) x->Render(window->camera->View(), window->camera->Proj());
}
