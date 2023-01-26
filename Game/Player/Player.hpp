#ifndef PLAYER_H
#define PLAYER_H

#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include <memory>

class Player : public Object
{
    public:
        // TODO: use some sort of resource map
        Player();

        void Start() override;
        void Tick(float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        std::shared_ptr<Material> material;
};

#endif
