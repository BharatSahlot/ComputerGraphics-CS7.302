#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"

#include <string>
#include <glm/glm.hpp>

class Material
{
    public:
        static void Unuse();
        static Material* MakeMaterial(const Shader* vertexShader, const Shader* fragmentShader);
        int Load(const Shader* vertexShader, const Shader* fragmentShader);

        int Use() const;

        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec4(const std::string& name, const glm::vec4& value) const;

        void SetUniformMat4(const std::string& name, const glm::mat4& mat) const;

        bool IsTransparent() const { return isTransparent; }

        ~Material();

    private:
        bool loaded = false;
        bool isTransparent = false;
        unsigned int shaderProgram;
};

#endif
