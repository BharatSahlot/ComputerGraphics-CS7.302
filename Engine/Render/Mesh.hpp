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
        std::string name;

        Mesh(vector<float> vertices, vector<unsigned int> indices);
        Mesh(vector<float> vertices, vector<unsigned int> indices, std::shared_ptr<Texture> texture, std::shared_ptr<Material> material);

        void Setup();
        void Render() const;
        void Render(glm::mat4 model, glm::mat4 view, glm::mat4 proj) const;

    private:
        std::shared_ptr<Material> material;

        // opengl objects for vertices, attributes and indices
        unsigned int VBO, VAO, EBO;
        int indices;

        std::shared_ptr<Texture> texture;
};

#endif
