#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

// assuming single threaded compile for now
static int success;
static char infoLog[512];

Shader* Shader::MakeShader(const char *path, int shaderType)
{
    std::string code;
    try
    {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(path);

        std::stringstream ss;
        ss << file.rdbuf();

        file.close();

        code = ss.str();
    } catch(std::ifstream::failure e)
    {
        std::cerr << "error reading shader file" << std::endl;
        return nullptr;
    }

    const char* code_str = code.c_str();
    Shader* shader = new Shader;

    shader->shaderObject = glCreateShader(shaderType);
    glShaderSource(shader->shaderObject, 1, &code_str, NULL);
    glCompileShader(shader->shaderObject);

    glGetShaderiv(shader->shaderObject, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader->shaderObject, 512, NULL, infoLog);
        std::cerr << "error compiling shader\n" << infoLog << std::endl;
        delete(shader);
        return nullptr;
    }
    
    return shader;
}

Shader::~Shader()
{
    glDeleteShader(this->shaderObject);
}

int Shader::Opengl_ID() const
{
    return shaderObject;
}
