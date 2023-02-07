#ifndef ZAPPER_H
#define ZAPPER_H

#include "Engine/Object.hpp"
#include "Engine/Utils/Timer.hpp"

class Zapper : public Object
{
    public:
        Zapper(float height, float speed, glm::vec3 col);

        void Start() override;
        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

        void SetYSpeed(float ySpeed);
        void SetRotSpeed(float speed);

    private:
        glm::vec3 col;

        float height;
        float xSpeed;
        float defYSpeed;
        float ySpeed;
        float rSpeed;

        int frame;
        Timer frameTimer;
};

#endif
