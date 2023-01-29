#ifndef ZAPPER_H
#define ZAPPER_H

#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"

class Zapper : public Object
{
    public:
        Zapper(float height);

        void Start() override;
        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        float height;
        int frame;
        Timer frameTimer;
};

#endif
