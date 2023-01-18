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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(int),
            indices.data(),
            GL_STATIC_DRAW);

    this->indices = indices.size();

    this->material = material;

    this->mMat = glm::mat4(1.0f);
}

void Mesh::Render(const glm::mat4& vpMat)
{
    this->material->Use();

    this->material->SetUniformMat4("vp", vpMat);
    this->material->SetUniformMat4("model", this->mMat);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, 0);
}
