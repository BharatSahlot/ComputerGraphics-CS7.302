#ifndef LEVEL_H
#define LEVEL_H

#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Game/Objects/Coin.hpp"
#include "Game/Objects/Text.hpp"
#include "Game/Objects/Zapper.hpp"
#include <memory>
#include <vector>
#include <stack>
#include <queue>
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

    float coinRadius;
    float coinSpawnInterval;
    float coinSpawnIntervalVar;

    float coinSpawnRadius;
    float coinSpawnRadiusVar;
};

class Level
{
    public:
        bool inMenu = true;
        bool started = false;
        bool hasEnded = false;
        bool playerDied = false;

        Level(std::shared_ptr<Camera> camera, LevelSettings settings);

        virtual int Load();

        void Start();
        void StartGame();
        void Tick(const Window& window, float deltaTime);
        virtual int Unload();

        void Render(const glm::mat4& view, const glm::mat4& proj);
        void EndLevel();

        const std::vector<std::shared_ptr<Zapper>>& GetActiveZappers() const { return zapperActive; }
        const std::vector<std::shared_ptr<Coin>>& GetActiveCoins() const { return coinsActive; }

        void PlayerCoinCollision(const std::shared_ptr<Coin>& coin);

    protected:
        float dist;
        int coinsCollected = 0;

        LevelSettings settings;
        std::shared_ptr<Camera> camera;
        std::multiset<std::shared_ptr<Object>, ZSorter> objects;
        std::multiset<std::shared_ptr<Object>, ZSorter> menuObjects;

        Timer levelTimer;

        std::shared_ptr<Text> distText;
        std::shared_ptr<Text> coinsText;

    private:
        void SpawnCoins(const Window& window);

        bool enterPressed = false;
        bool levelEnded = false; // hasEnded gets true after fading is finished

        int fadeDir;
        Timer zapperSpawnTimer;
        std::queue<std::shared_ptr<Zapper>> zapperPool;
        std::vector<std::shared_ptr<Zapper>> zapperActive;


        Timer coinSpawnTimer;
        std::queue<std::shared_ptr<Coin>> coinsPool;
        std::vector<std::shared_ptr<Coin>> coinsActive;
};

#endif
