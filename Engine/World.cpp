#include "World.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/Window/Window.hpp"
#include "glm/gtx/norm.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

World::World(std::shared_ptr<Window> window) : window(window)
{
    resourceManager = std::unique_ptr<ResourceManager>(ResourceManager::CreateResourceManager(window->GetGLFWwindow(), this));

    primitive = new Primitive(this);
}

void World::Start()
{
    for(auto x: objects) x->Start();
}

void World::Tick(float deltaTime) const
{
    for(auto x: objects) x->Tick(deltaTime);
    for(auto x: uiObjs) x->Tick(deltaTime);
}

void World::Render()
{
    window->camera->Use(glm::vec2(window->Width(), window->Height()), glm::vec3(0, 0, 1.f));

    glm::vec3 cz = window->camera->Position();
    std::sort(objects.begin(), objects.end(), [cz](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool {
        bool ta = a->IsTransparent();
        bool tb = b->IsTransparent();

        // a is transparent b is not
        if(ta && !tb) return false;

        // a is not b is
        if(!ta && tb) return true;

        // a and b both(not)
        float az = glm::distance2(cz, a->transform->GetWorldPosition());
        float bz = glm::distance2(cz, b->transform->GetWorldPosition());
        if(ta && tb) return az > bz;
        return az < bz;
    });
    for(auto x: objects) x->Render(window->camera->View(), window->camera->Proj());

    while(!lineQueue.empty())
    {
        auto [start, end, col] = lineQueue.front();
        lineQueue.pop();

        primitive->DrawLine(start, end, col);
    }

    while(!boxQueue.empty())
    {
        auto [center, extents, col] = boxQueue.front();
        boxQueue.pop();

        primitive->DrawBox(center, extents, col);
    }

    for(auto x: uiObjs) x->Render();
}

void World::Render(const Camera& camera)
{
    camera.Use(glm::vec2(window->Width(), window->Height()), glm::vec3(0, 0, 1.f));
    for(auto x: objects) x->Render(camera.View(), camera.Proj());
}

void World::DrawRotatedBox(std::vector<glm::vec3> points)
{
    lineQueue.emplace(points[0], points[1], glm::vec3(1));
    lineQueue.emplace(points[1], points[2], glm::vec3(1));
    lineQueue.emplace(points[2], points[3], glm::vec3(1));
    lineQueue.emplace(points[3], points[0], glm::vec3(1));

    lineQueue.emplace(points[4], points[5], glm::vec3(1));
    lineQueue.emplace(points[5], points[6], glm::vec3(1));
    lineQueue.emplace(points[6], points[7], glm::vec3(1));
    lineQueue.emplace(points[7], points[4], glm::vec3(1));

    lineQueue.emplace(points[0], points[4], glm::vec3(1));
    lineQueue.emplace(points[1], points[5], glm::vec3(1));
    lineQueue.emplace(points[2], points[6], glm::vec3(1));
    lineQueue.emplace(points[3], points[7], glm::vec3(1));
}
