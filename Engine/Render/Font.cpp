#include "Font.hpp"
#include "Globals.hpp"
#include <iostream>
#include <glad/glad.h>

Font* Font::LoadFont(const std::string& file, int size, std::shared_ptr<Material> mat)
{
    Font* font = new Font;
    if(font->Load(file, size, mat) == -1)
    {
        delete font;
        return nullptr;
    }

    glGenVertexArrays(1, &font->VAO);
    glGenBuffers(1, &font->VBO);
    glBindVertexArray(font->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, font->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    font->mat = mat;
    font->hasShadow = false;

    return font;
}

int Font::Load(const std::string& file, int size, std::shared_ptr<Material> mat)
{
    FT_Face face;
    if(FT_New_Face(ft, file.c_str(), 0, &face))
    {
        std::cerr << "Failed to load font " << file << std::endl;
        return -1;
    }

    this->face = face;
    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // remove byte alignment requirement
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "error loading glyph for " << c << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 
                0, 
                GL_RED, 
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);

    hasShadow = false;
    this->mat = mat;
    this->size = size;
    return 0;
}

void Font::Setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::RenderText(std::string text, float x, float y, float scale, glm::mat4 proj, glm::vec4 col)
{
    mat->Use();
    mat->SetUniformMat4("proj", proj);
    mat->SetVec4("textColor", col);
    mat->SetInt("text", 0);

    if(!hasShadow)
    {
        mat->SetVec4("shadowColor", glm::vec4(0, 0, 0, 0));
        mat->SetFloat("shadowDistance", 0);
    } else 
    {
        mat->SetVec4("shadowColor", shadowColor);
        mat->SetFloat("shadowDistance", shadowDistance);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for(char c: text)
    {
        auto ch = characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        x += (ch.advance >> 6) * scale;
    }
}

glm::vec2 Font::GetTextDims(std::string text, float scale) const
{
    glm::vec2 res(0, 0);
    float x = 0, y = 0;
    for(char c: text)
    {
        auto ch = characters.at(c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        res.x = xpos + w;
        res.y = std::max(res.y, ypos + h);

        x += (ch.advance >> 6) * scale;
    }
    return res;
}
