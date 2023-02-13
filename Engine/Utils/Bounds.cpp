#include "Bounds.hpp"

#include <iostream>

Bounds Bounds::FromExtents(glm::vec3 center, glm::vec3 extents)
{
    Bounds b;
    b.center = center;
    b.extents = extents;
    b.min = b.center - b.extents;
    b.max = b.center + b.extents;
    return b;
}

Bounds Bounds::FromMinMax(glm::vec3 mn, glm::vec3 mx)
{
    Bounds b;
    b.max = mx;
    b.min = mn;
    b.extents = (mx - mn) * 0.5f;
    b.center = (mx + mn) * 0.5f;
    return b;
}

void Bounds::Join(Bounds b)
{
    min = glm::vec3(std::min(b.min.x, min.x),
                    std::min(b.min.y, min.y),
                    std::min(b.min.z, min.z));

    max = glm::vec3(std::max(b.max.x, max.x),
                    std::max(b.max.y, max.y),
                    std::max(b.max.z, max.z));
    extents = (max - min) * 0.5f;
    center = (max + min) * 0.5f;
}

std::vector<glm::vec3> Bounds::GetRotatedBox(glm::mat4 model) const
{
    std::vector<glm::vec3> res;
    res.push_back(model * glm::vec4(center - extents * glm::vec3( 1,  1,  1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3(-1,  1,  1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3(-1, -1,  1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3( 1, -1,  1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3( 1,  1, -1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3(-1,  1, -1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3(-1, -1, -1), 1));
    res.push_back(model * glm::vec4(center - extents * glm::vec3( 1, -1, -1), 1));
    return res;
}
