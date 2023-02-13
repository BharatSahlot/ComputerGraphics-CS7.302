#ifndef BOX_H
#define BOX_H

#include "Engine/Render/Material.hpp"
#include <glm/glm.hpp>
#include <memory>

class World;

class Primitive
{
    public:
        Primitive(World* world);

        // Draw line segment from start to end
        void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1, 1, 1));

        // Center of the box and extents = size / 2, color(default white)
        void DrawBox(glm::vec3 center, glm::vec3 extents, glm::vec3 color = glm::vec3(1, 1, 1));

        // Center of the sphere, radius and color(default white)
        // void DrawSphere(glm::vec3 center, float radius, glm::vec3 color = glm::vec3(1, 1, 1));

        // Center of the plane and x,y scale of the plane
        // void DrawPlane(glm::vec3 center, glm::vec2 scale);

    private:
        World* world;

        std::shared_ptr<Material> mat;

        unsigned int lineVAO, lineVBO;
};

#endif
