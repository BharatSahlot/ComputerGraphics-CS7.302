#ifndef WORLD_H
#define WORLD_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Render/Primitive/Primitive.hpp"
#include "Engine/Resource/ResourceManager.hpp"
#include "Engine/UI/Text.hpp"
#include <memory>
#include <optional>
#include <iostream>

class Window;

class World
{
    public:
        World(std::shared_ptr<Window> window);

        ResourceManager& GetResourceManager() const { return *resourceManager; }

        Window& GetWindow() const
        {
            return *window;
        }

        virtual void Start() {};
        virtual void Tick(float deltaTime) const;
        virtual void Render();
        virtual void Render(const Camera& camera);

        template<typename T, typename... Us>
        std::shared_ptr<T> Instantiate(std::string name, Us... args)
        {
            std::shared_ptr<T> ptr(new T(this, name, args...));
            objects.push_back(ptr);
            return ptr;
        }

        template<typename T, typename... Us>
        std::shared_ptr<T> InstantiateUIObject(std::string name, Us... args)
        {
            std::shared_ptr<T> ptr(new T(this, name, args...));
            uiObjs.push_back(ptr);
            return ptr;
        }

        // returns first object with the name name
        template<typename T>
        T* GetObjectByName(std::string name)
        {
            for(auto x: objects)
            {
                if(x->name == name) return static_cast<T*>(x.get());
            }
            return nullptr;
        }

        template<typename T>
        T* GetUIObjectByName(std::string name)
        {
            for(auto x: uiObjs)
            {
                if(x->name == name) return static_cast<T*>(x.get());
            }
            return nullptr;
        }

        void DrawBox(glm::vec3 center, glm::vec3 extents, glm::vec3 color = glm::vec3(1, 1, 1)) const
        {
            primitive->DrawBox(center, extents, color);
        }

        void DrawRotatedBox(std::vector<glm::vec3> points) const;

    protected:
        std::shared_ptr<Window> window;

        // should really be unique ptr
        std::vector<std::shared_ptr<Object>> objects;

        Primitive* primitive = nullptr;
        std::unique_ptr<ResourceManager> resourceManager;

        std::vector<std::shared_ptr<UIObject>> uiObjs;
};

#endif
