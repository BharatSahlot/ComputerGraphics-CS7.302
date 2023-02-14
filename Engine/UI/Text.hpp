#ifndef TEXT_H
#define TEXT_H

#include "Engine/Object.hpp"
#include "Engine/Render/Font.hpp"

enum AnchorType
{
    BottomLeft,
    BottomRight,
    TopLeft,
    TopRight,
    Center,
    CenterTop,
    CenterBottom,
};

struct Anchor
{
    AnchorType anchorType;
    glm::vec2 padding;
    float scale;
};

class Text
{
    public:
        World* world;
        std::string name;

        Text(World* world, std::string name, std::shared_ptr<Font> font, Anchor anchor);
        // Text(World* world, std::string name, std::string font, Anchor anchor);

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
