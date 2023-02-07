#ifndef MESH_H
#define MESH_H

#include "Material.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Mesh
{
    template<typename T>
    using vector = std::vector<T>;

    public:
        Mesh(vector<float> vertices, vector<int> indices, bool hasNormals);
        void Render();

    private:
        // opengl objects for vertices, attributes and indices
        unsigned int VBO, VAO, EBO;
        int indices;

        // no need to store in ram for now
        // vector<float> vertices;
        // vector<int> indices;
};

#endif
