#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glm/glm.hpp>

#include <glad/glad.h>

class Texture
{
    public:
        static Texture* MakeTexture(const std::string& file, int filtering = GL_NEAREST);

        void Use(int unit);
        glm::vec2 GetDims() const { return dims; }

    private:
        unsigned int texture;
        glm::vec2 dims;
};

#endif
