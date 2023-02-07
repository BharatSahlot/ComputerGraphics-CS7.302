#include "Text.hpp"
#include "Globals.hpp"

Text::Text(float left, float top, float scale, glm::vec3 col) : Object("text", nullptr, TextMat)
{
    text = "";
    this->left = left;
    this->top = top;
    this->scale = scale / 200.f;
    this->col = col;
}

void Text::Tick(const Window &window, float deltaTime)
{
    renderX = left;
    renderY = 1 - top;

    glm::vec2 point = window.ViewportPointToWorld(glm::vec3(renderX, renderY, 0));
    renderX = point.x;
    renderY = point.y;
}

void Text::Render(const glm::mat4 &viewMat, const glm::mat4 &projMat)
{
    font->RenderText(text, 
            renderX, 
            renderY, 
            scale,
            projMat,
            col);
}
