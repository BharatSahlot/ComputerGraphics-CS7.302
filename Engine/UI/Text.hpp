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

        void SetAnchor(Anchor anchor) { this->anchor = anchor; }
        void SetColor(glm::vec3 col) { this->col = col; }
        void SetText(std::string str) { this->str = str; }

    private:
        std::shared_ptr<Font> font;
        Anchor anchor;
        glm::vec3 col;
        std::string str;
};

#endif
