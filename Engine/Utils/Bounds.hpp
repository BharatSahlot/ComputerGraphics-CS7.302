#ifndef BOUNDS_H
#define BOUNDS_H

#include <glm/glm.hpp>

#include <vector>

struct Bounds
{
    static Bounds FromExtents(glm::vec3 center, glm::vec3 extents);
    static Bounds FromMinMax(glm::vec3 mn, glm::vec3 mx);

    void Join(Bounds b);
    std::vector<glm::vec3> GetRotatedBox(glm::mat4 model) const;

    glm::vec3 center, extents, max, min;
};

#endif
