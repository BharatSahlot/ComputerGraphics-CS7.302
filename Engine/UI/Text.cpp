#include "Text.hpp"

#include "Engine/Window/Window.hpp"
#include "Engine/World.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Text::Text(World* world, std::string name, std::shared_ptr<Font> font, Anchor anchor) : world(world), 
    name(name), 
    anchor(anchor),
    font(font),
    col(glm::vec3(1, 1, 1)),
    str("")
{
}

// Text::Text(World* world, std::string name, std::string font, Anchor anchor) : world(world), 
//     name(name), 
//     anchor(anchor),
//     col(glm::vec3(1, 1, 1)),
//     str("")
// {
//     this->font = world->GetResourceManager().Get<Font>(font);
// }

void Text::Render()
{
    glm::vec2 screenDims(world->GetWindow().Width(), world->GetWindow().Height());
    glm::vec2 textDims = font->GetTextDims(str, anchor.scale);

    float x = anchor.padding.x;
    float y = anchor.padding.y;
    switch (anchor.anchorType) {
        case BottomLeft:
            break;
        case BottomRight:
            x = screenDims.x - textDims.x - x;
            break;
        case TopLeft:
            y = screenDims.y - textDims.y - y;
            break;
        case TopRight:
            x = screenDims.x - textDims.x - x;
            y = screenDims.y - textDims.y - y;
            break;
        case Center:
            x = ((screenDims.x - textDims.x) / 2.f) + x;
            y = ((screenDims.y - textDims.y) / 2.f) + y;
            break;
        case CenterTop:
            x = ((screenDims.x - textDims.x) / 2.f) + x;
            y = screenDims.y - textDims.y - y;
            break;
        case CenterBottom:
            x = ((screenDims.x - textDims.x) / 2.f) + x;
            break;
    }

    glm::mat4 proj = glm::ortho(0.f, screenDims.x, 0.f, screenDims.y);
    font->RenderText(str, x, y, anchor.scale, proj, col);
}
