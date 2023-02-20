#include "UIObject.hpp"

#include "Engine/World.hpp"
#include "Engine/Window/Window.hpp"

glm::vec2 UIObject::CalculatePosition(Anchor anchor, glm::vec2 dims)
{
    glm::vec2 screenDims(world->GetWindow().Width(), world->GetWindow().Height());

    float x = anchor.padding.x;
    float y = anchor.padding.y;
    switch (anchor.anchorType) {
        case BottomLeft:
            break;
        case BottomRight:
            x = screenDims.x - dims.x - x;
            break;
        case TopLeft:
            y = screenDims.y - dims.y - y;
            break;
        case TopRight:
            x = screenDims.x - dims.x - x;
            y = screenDims.y - dims.y - y;
            break;
        case Center:
            x = ((screenDims.x - dims.x) / 2.f) + x;
            y = ((screenDims.y - dims.y) / 2.f) + y;
            break;
        case CenterTop:
            x = ((screenDims.x - dims.x) / 2.f) + x;
            y = screenDims.y - dims.y - y;
            break;
        case CenterBottom:
            x = ((screenDims.x - dims.x) / 2.f) + x;
            break;
    }
    return glm::vec2(x, y);
}
