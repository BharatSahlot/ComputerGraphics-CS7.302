#ifndef SKY_H
#define SKY_H

#include "Engine/Render/Material.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class World;

class Sky
{
    public:
        Sky(World* world);

        int Load(std::vector<std::string> faces);
        void Setup();

        void Render(glm::mat4 view, glm::mat4 proj) const;

    private:
        unsigned int texture, VAO, VBO;
        std::shared_ptr<Material> mat;
};

#endif
