#include "Mesh.hpp"
#include "Material.hpp"

#include <glad/glad.h>
#include <memory>
#include <iostream>

Mesh::Mesh(vector<float> vertices, vector<unsigned int> indices, std::shared_ptr<Texture> texture, std::shared_ptr<Material> material)
{
    GenBuffers(vertices, indices);
    this->texture = texture;
    this->material = material;

    this->vertices = vertices;
    faces = indices;
}

Mesh::Mesh(vector<float> vertices, vector<unsigned int> indices, std::shared_ptr<Texture> texture, std::shared_ptr<Material> material, Bounds bounds)
{
    GenBuffers(vertices, indices);
    this->texture = texture;
    this->material = material;
    this->bounds = bounds;

    this->vertices = vertices;
    faces = indices;
}

Mesh::Mesh(vector<float> vertices, vector<unsigned int> indices)
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(int),
            indices.data(),
            GL_STATIC_DRAW);

    this->indices = indices.size();

    this->vertices = vertices;
    faces = indices;
}

void Mesh::GenBuffers(vector<float> vertices, vector<unsigned int> indices)
{
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW);

    this->indices = indices.size();
}

void Mesh::Setup()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
}

void Mesh::Render() const
{
    if(material)
    {
        material->Use();
    }
    if(texture)
    {
        texture->Use(0);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    Material::Unuse();
}

void Mesh::Render(glm::mat4 model, glm::mat4 view, glm::mat4 proj) const
{
    if(material)
    {
        if(material->Use() == -1)
        {
            std::cerr << "Error rendering mesh " << name << std::endl;
        }
        material->SetUniformMat4("model", model);
        material->SetUniformMat4("view", view);
        material->SetUniformMat4("proj", proj);
        material->SetInt("texture1", 0);
        material->SetVec3("col", glm::vec3(1, 1, 1));
        material->SetFloat("oneMinusAlpha", 0);
    }
    if(texture)
    {
        texture->Use(0);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Render(glm::mat4 model, glm::mat4 view, glm::mat4 proj, float oneMinusAlpha, glm::vec3 color) const
{
    if(material)
    {
        if(material->Use() == -1)
        {
            std::cerr << "Error rendering mesh " << name << std::endl;
        }
        material->SetUniformMat4("model", model);
        material->SetUniformMat4("view", view);
        material->SetUniformMat4("proj", proj);
        material->SetInt("texture1", 0);
        material->SetVec3("col", color);
        material->SetFloat("oneMinusAlpha", oneMinusAlpha);
    }
    if(texture)
    {
        texture->Use(0);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
