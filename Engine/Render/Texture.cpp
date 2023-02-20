#include "Texture.hpp"
#include "Engine/Utils/Util.hpp"

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture* Texture::MakeTexture(const std::string& file, int filtering)
{
    Texture* texture = new Texture;
    if(texture->Load(file, filtering) == -1)
    {
        delete texture;
        return nullptr;
    }
    return texture;
}

int Texture::Load(const std::string &file, int filtering)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nChannels;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nChannels, 0);

    if(!data)
    {
        // somehow get the error
        return -1;
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

    if(file.substr(file.find_last_of('.'), file.size()) == ".jpg")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);

    texture = tex;
    dims = glm::vec2(width, height);
    return 0;
}

void Texture::Use(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}
