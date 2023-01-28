#include "Texture.hpp"
#include "Engine/Utils/Util.hpp"

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture* Texture::MakeTexture(const std::string& file, int filtering)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nChannels;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nChannels, 0);

    if(!data)
    {
        // somehow get the error
        return nullptr;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    // set wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set filtering to nearest, since we want pixel look in this game
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    Texture* texture = new Texture;
    texture->texture = tex;
    texture->dims = glm::vec2(width, height);
    return texture;
}

void Texture::Use(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
