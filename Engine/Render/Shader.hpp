#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
    public:
        static Shader* MakeShader(const std::string& file, int shaderType);
        int Opengl_ID() const;
        bool IsTransparent() const { return isTransparent; }
        ~Shader();

    private:
        // opengl shader object
        unsigned int shaderObject;
        bool isTransparent;
};

#endif
