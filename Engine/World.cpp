#include "World.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/Window/Window.hpp"

World::World(std::shared_ptr<Window> window)
{
    this->window = window;
    resourceManager = std::unique_ptr<ResourceManager>(ResourceManager::CreateResourceManager(window->GetGLFWwindow(), this));
}
