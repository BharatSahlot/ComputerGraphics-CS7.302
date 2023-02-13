#ifndef WORLD_H
#define WORLD_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Primitive/Primitive.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include <memory>
#include <optional>

class Window;

class World
{
    public:
        World(std::shared_ptr<Window> window);

        ResourceManager& GetResourceManager() const { return *resourceManager; }
        Window& GetWindow() const { return *window; }

        void Tick(float deltaTime) const;
        void Render();
        void Render(const Camera& camera);

        template<typename T, class... Us>
        std::shared_ptr<T> Instantiate(std::string name, Us... args)
        {
            std::shared_ptr<T> ptr(new T(this, name, args...));
            objects.push_back(ptr);
            return ptr;
        }

        // returns first object with the name name
        template<typename T>
        const T* GetObjectByName(std::string name)
        {
            for(auto x: objects)
            {
                if(x->name == name) return static_cast<const T*>(x.get());
            }
            return nullptr;
        }

        void DrawBox(glm::vec3 center, glm::vec3 extents, glm::vec3 color = glm::vec3(1, 1, 1)) const
        {
            primitive->DrawBox(center, extents, color);
        }

        void DrawRotatedBox(std::vector<glm::vec3> points) const;

    private:
        Primitive* primitive = nullptr;
        std::unique_ptr<ResourceManager> resourceManager;
        std::shared_ptr<Window> window;

        // should really be unique ptr
        std::vector<std::shared_ptr<Object>> objects;
};

#endif
