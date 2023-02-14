#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <glm/glm.hpp>
#include <string>

class World;

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

class UIObject
{
    public:
        World* world;
        std::string name;

        virtual void Tick(float deltaTime) {}
        virtual void Render() {}

    protected:
        UIObject(World* world, std::string name) : world(world), name(name) {}

        glm::vec2 CalculatePosition(Anchor anchor, glm::vec2 dims);
};

#endif
