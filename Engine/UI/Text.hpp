#ifndef TEXT_H
#define TEXT_H

#include "Engine/Object.hpp"
#include "Engine/Render/Font.hpp"
#include "Engine/UI/UIObject.hpp"

class Text : public UIObject
{
    public:
        Text(World* world, std::string name, std::shared_ptr<Font> font, Anchor anchor);
        Text(World* world, std::string name, std::string font, Anchor anchor);

        void Render();

        Text* SetAnchor(Anchor anchor) { this->anchor = anchor; return this; }
        Text* SetColor(glm::vec3 col) { this->col = glm::vec4(col, 1.f); return this; }
        Text* SetColor(glm::vec4 col) { this->col = col; return this; }
        Text* SetText(std::string str) { this->str = str;  return this; }

        Text* EnableShadow() { hasShadow = true; return this; }
        Text* DisableShadow() { hasShadow = false; return this; }
        Text* SetShadow(float distance, glm::vec4 col)
        {
            shadowDistance = distance;
            shadowColor = col;
            return this;
        }

    private:
        std::shared_ptr<Font> font;
        Anchor anchor;
        glm::vec4 col;
        std::string str;

        bool hasShadow;
        float shadowDistance;
        glm::vec4 shadowColor;
};

#endif
