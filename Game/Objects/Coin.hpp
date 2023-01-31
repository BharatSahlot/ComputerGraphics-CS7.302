#ifndef COIN_H
#define COIN_H

#include "Engine/Object.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Timer.hpp"

class Coin : public Object
{
    public:
        Coin(float radius, float speed);
        void Start() override;
        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        float speed;
        Texture* sheet;

        int frame;
        Timer frameTimer;
};

#endif
