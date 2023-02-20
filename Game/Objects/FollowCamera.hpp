#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

#include "Engine/Camera.hpp"
#include "Engine/Utils/Timer.hpp"

class FollowCamera : public Camera
{
    public:
        FollowCamera(World* world, std::string name, std::string player, glm::vec3 offset, glm::vec3 hudOffset);

        void Start() override;
        void Tick(float deltaTime) override;

    private:
        Object* player;

        bool hudView = false;
        Timer keyTimer;

        glm::vec3 front, targetFront;
        glm::vec3 up;
        glm::vec3 offset;
        glm::vec3 targetPos;

        glm::vec3 hudOffset;
};

#endif
