#ifndef SHADER_H
#define SHADER_H

// workflow: compile all shaders in the beginning, then link to materials and then delete the shaders
class Shader
{
    public:
        static Shader* MakeShader(const char* file, int shaderType);
        int Opengl_ID() const;
        ~Shader();

    private:
        // opengl shader object
        unsigned int shaderObject;
};

#endif
