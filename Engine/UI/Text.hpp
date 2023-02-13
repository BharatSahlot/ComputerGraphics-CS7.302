#ifndef TEXT_H
#define TEXT_H

#include "Engine/Object.hpp"

enum AnchorType
{
    BottomLeft,
    BottomRight,
    TopLeft,
    TopRight
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
        Text(World* world, std::string name, Anchor anchor);

        void Render(const glm::mat4 &projMat);

        void SetAnchor(Anchor anchor) { this->anchor = anchor; }
        void SetColor(glm::vec3 col) { this->col = col; }
        void SetText(std::string str) { this->str = str; }

    private:
        Anchor anchor;
        glm::vec3 col;
        std::string str;
};

#endif
