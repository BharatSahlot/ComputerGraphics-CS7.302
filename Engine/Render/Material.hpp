#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"

#include <string>

class Material
{
    public:
        static Material* MakeMaterial(const Shader* vertexShader, const Shader* fragmentShader);

        void Use();

        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;

        ~Material();

    private:
        unsigned int shaderProgram;
};

#endif
