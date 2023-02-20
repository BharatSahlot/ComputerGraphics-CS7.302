#ifndef BAR_H
#define BAR_H

#include "Engine/Render/Material.hpp"
#include "Engine/UI/Text.hpp"
#include "Engine/UI/UIObject.hpp"

class Bar : public UIObject
{
    public:
        Bar(World* world, std::string name, Anchor anchor, float width, float height);

        void SetAnchor(Anchor a) { anchor = a; }
        void SetWidth(float w) { width = w; }
        void SetHeight(float h) { height = h; }
        void SetFillAmount(float a) { fillAmount = a; }
        void SetColor(glm::vec3 c) { col = c; }
        void SetBorder(float b) { border = b; }

        void Render() override;

    private:
        std::shared_ptr<Material> mat;
        unsigned int VAO, VBO;

        Anchor anchor;
        float width;
        float height;
        float fillAmount;
        float border;
        
        glm::vec3 col;
};

#endif
