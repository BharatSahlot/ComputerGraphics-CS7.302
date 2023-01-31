#ifndef LEVEL_H
#define LEVEL_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Game/Objects/Text.hpp"
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

struct LevelSettings
{
    float duration;
    float speedModifier;

    float zapperSpawnInterval;
    float zapperSpawnIntervalVar;

    float zapperHeight;
    // zapper height variance, new zapper height = zapperHeight + [-zapperHeightVar, zapperHeightVar];
    float zapperHeightVar;

    float zapperRotSpeed;
    float zapperRotSpeedVar;
    
    float zapperYSpeed;
    float zapperYSpeedVar;

    glm::vec3 zapperCol;
};

class Level
{
    public:
        bool started = false;
        bool hasEnded = false;
        bool playerDied = false;

        Level(std::shared_ptr<Camera> camera, LevelSettings settings);

        virtual int Load();

        void Start();
        void Tick(const Window& window, float deltaTime);
        virtual int Unload();

        void Render(const glm::mat4& view, const glm::mat4& proj);
        void EndLevel();

        const std::vector<std::shared_ptr<Zapper>>& GetActiveZappers() const { return zapperActive; }

    protected:
        float dist;

        LevelSettings settings;
        std::shared_ptr<Camera> camera;
        std::multiset<std::shared_ptr<Object>, ZSorter> objects;
        Timer levelTimer;

        std::shared_ptr<Text> distText;
        std::shared_ptr<Text> coinsText;

    private:
        Timer zapperSpawnTimer;
        float zapperSpawnInterval;
        std::stack<std::shared_ptr<Zapper>> zapperPool;
        std::vector<std::shared_ptr<Zapper>> zapperActive;
};

#endif
