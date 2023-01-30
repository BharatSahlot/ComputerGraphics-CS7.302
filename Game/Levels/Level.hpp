#ifndef LEVEL_H
#define LEVEL_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Game/Objects/Zapper.hpp"
#include <memory>
#include <vector>
#include <stack>
#include <set>

struct ZSorter
{
    bool operator()(std::shared_ptr<Object> a, std::shared_ptr<Object> b) const
    {
        float az = a->transform->GetWorldPosition().z;
        float bz = b->transform->GetWorldPosition().z;
        return az < bz;
    }
};

class Level
{
    public:
        bool started = false;

        Level(std::shared_ptr<Camera> camera);

        virtual int Load();

        void Start();
        void Tick(const Window& window, float deltaTime);
        virtual int Unload();

        void Render(const glm::mat4& view, const glm::mat4& proj);

        // TODO:
        // std::weak_ptr<T> GetObjectsOfType(T type);

    protected:
        std::shared_ptr<Camera> camera;
        // std::vector<std::shared_ptr<Object>> objects;
        std::multiset<std::shared_ptr<Object>, ZSorter> objects;

    private:
        Timer zapperSpawnTimer;
        float zapperSpawnInterval;
        std::stack<std::shared_ptr<Zapper>> zapperPool;
        std::vector<std::shared_ptr<Zapper>> zapperActive;
};

#endif
