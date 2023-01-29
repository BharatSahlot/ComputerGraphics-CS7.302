#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Engine/Object.hpp"
#include <string>

class Background : public Object
{
    public:
        Background(std::string name, const std::string& file, float speed, float z);

        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        float speed;
};

#endif
