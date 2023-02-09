#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"

#include <string>
#include <glm/glm.hpp>

class Material
{
    public:
        static Material* MakeMaterial(const Shader* vertexShader, const Shader* fragmentShader);
        int Load(const Shader* vertexShader, const Shader* fragmentShader);

        void Use();

        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec3(const std::string& name, const glm::vec3& value) const;

        void SetUniformMat4(const std::string& name, const glm::mat4& mat) const;

        ~Material();

    private:
        unsigned int shaderProgram;
};

#endif
