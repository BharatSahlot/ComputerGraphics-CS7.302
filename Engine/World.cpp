#include "World.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/Window/Window.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

World::World(std::shared_ptr<Window> window)
{
    this->window = window;
    resourceManager = std::unique_ptr<ResourceManager>(ResourceManager::CreateResourceManager(window->GetGLFWwindow(), this));

    primitive = new Primitive(this);
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

void World::Render(const Camera& camera)
{
    for(auto x: objects) x->Render(camera.View(), camera.Proj());
}

void World::DrawRotatedBox(std::vector<glm::vec3> points) const
{
    static bool once = true;
    if(once)
    {
        for(auto x: points)
        {
            std::cout << x.x << ' ' << x.y << ' ' << x.z << std::endl;
        }
    }
    once = false;
    primitive->DrawLine(points[0], points[1]);
    primitive->DrawLine(points[1], points[2]);
    primitive->DrawLine(points[2], points[3]);
    primitive->DrawLine(points[3], points[0]);

    primitive->DrawLine(points[4], points[5]);
    primitive->DrawLine(points[5], points[6]);
    primitive->DrawLine(points[6], points[7]);
    primitive->DrawLine(points[7], points[4]);

    primitive->DrawLine(points[0], points[4]);
    primitive->DrawLine(points[1], points[5]);
    primitive->DrawLine(points[2], points[6]);
    primitive->DrawLine(points[3], points[7]);
}
