#ifndef FONT_H
#define FONT_H

#include "Engine/Engine.hpp"

#include <glm/glm.hpp>

#include <map>
#include <string>

struct Character
{
    unsigned int texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Font
{
    public:
        static Font* LoadFont(const std::string& file, int size);
        void RenderText(std::string text, float x, float y, float scale, glm::mat4 proj, glm::vec3 col);

    private:
        FT_Face face;
        unsigned int VAO;
        unsigned int VBO;
        std::map<char, Character> characters;
};

#endif
