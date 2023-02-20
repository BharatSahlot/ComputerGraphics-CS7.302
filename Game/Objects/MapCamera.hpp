#ifndef MAP_CAMERA_H
#define MAP_CAMERA_H

#include "Engine/Camera.hpp"

class MapCamera : public Camera
{
    public:
        MapCamera(World* world, std::string name, std::string player, glm::vec3 offset);

        void Start() override;
        void Tick(float deltaTime) override;

    private:
        Object* player;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 offset;
};

#endif
