#include "Material.hpp"

#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// TODO:just do this better
static char infoLog[512];
static int success;

Material* Material::MakeMaterial(const Shader* vertexShader, const Shader* fragmentShader)
{
    int id = glCreateProgram();
    glAttachShader(id, vertexShader->Opengl_ID());
    glAttachShader(id, fragmentShader->Opengl_ID());
    glLinkProgram (id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cerr << "error linking program\n" << infoLog << std::endl;
        return nullptr;
    }

    Material* material = new Material;
    material->shaderProgram = id;
    return material;
}

void Material::Use()
{
    glUseProgram(this->shaderProgram);
}

void Material::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), (int) value);
}

void Material::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}

void Material::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}

void Material::SetUniformMat4(const std::string &name, const glm::mat4 &mat)
{
    int loc = glGetUniformLocation(this->shaderProgram, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

Material::~Material()
{
    glDeleteProgram(this->shaderProgram);
}
