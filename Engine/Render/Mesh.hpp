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
        glm::mat4 mMat;

        Mesh(vector<float> vertices, vector<int> indices, std::shared_ptr<Material> material);
        void Render(const glm::mat4& vpMat);

    private:
        // opengl objects for vertices, attributes and indices
        unsigned int VBO, VAO, EBO;
        int indices;

        std::shared_ptr<Material> material;

        // no need to store in ram for now
        // vector<float> vertices;
        // vector<int> indices;
};

#endif
