#ifndef TEXT_H
#define TEXT_H

#include "Engine/Object.hpp"

class Text : public Object
{
    public:
        // left and top in percentage
        Text(float left, float top, float scale, glm::vec3 col);
        void SetText(std::string txt) { text = txt; }
        void Tick(const Window &window, float deltaTime) override;
        void Render(const glm::mat4 &viewMat, const glm::mat4 &projMat) override;

    private:
        float left, top, scale;
        glm::vec3 col;

        float renderX, renderY;
        std::string text;
};

#endif
