#include "Mesh.hpp"
#include "Material.hpp"

#include <glad/glad.h>
#include <memory>

Mesh::Mesh(vector<float> vertices, vector<int> indices, std::shared_ptr<Material> material)
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(int),
            indices.data(),
            GL_STATIC_DRAW);

    this->indices = indices.size();

    this->material = material;
}

void Mesh::Render()
{
    this->material->Use();
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, 0);
}
