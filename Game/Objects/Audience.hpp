#ifndef AUDIENCE_H
#define AUDIENCE_H

#include "Engine/Object.hpp"
#include "Engine/Render/Material.hpp"
#include "Engine/Render/Texture.hpp"
#include "Engine/Utils/Timer.hpp"
#include <vector>

class Audience : public Object
{
    public:
        Audience(World* world, std::string name, int count, float size);

        void Start() override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        int count;
        float size;

        Mesh* mesh;
        std::shared_ptr<Texture> spriteSheet;
        std::shared_ptr<Material> material;

        std::vector<glm::vec3> positions;

        std::vector<unsigned int> frames;

        Timer frameTimer;
};

#endif
