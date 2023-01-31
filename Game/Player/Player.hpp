#ifndef PLAYER_H
#define PLAYER_H

#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Mesh.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Window/Window.hpp"
#include "Game/Levels/Level.hpp"
#include <memory>

class Player : public Object
{
    public:
        Player(Level* level);

        void Start() override;
        void Tick(const Window& window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        void SetCurrentSheet(Texture* sheet);

        Level* level;
        Texture* idleSheet;
        Texture* walkSheet;
        Texture* flySheet;

        Texture* currentSheet;

        glm::vec3 velocity;
        glm::vec3 col;

        Timer frameTimer;
        int frame;
};

#endif
