#ifndef MESH_H
#define MESH_H

#include "Engine/Render/Texture.hpp"
#include "Material.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Mesh
{
    template<typename T>
    using vector = std::vector<T>;

    public:
        Mesh(vector<float> vertices, vector<unsigned int> indices);
        Mesh(vector<float> vertices, vector<unsigned int> indices, std::shared_ptr<Texture> texture);

        void Setup();
        void Render() const;

    private:
        // opengl objects for vertices, attributes and indices
        unsigned int VBO, VAO, EBO;
        int indices;

        std::shared_ptr<Texture> texture;
        // no need to store in ram for now
        // std::vector<float> vertices;
        // std::vector<int> indices;
};

#endif
