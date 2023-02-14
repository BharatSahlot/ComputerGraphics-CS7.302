#include "LoadingBar.hpp"
#include "Engine/UI/Bar.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Engine/World.hpp"

LoadingBar::LoadingBar(World* world, std::string name, std::function<float(std::string* status)> statusFunc) : UIObject(world, name)
{
    this->statusFunc = statusFunc;

    bar = world->InstantiateUIObject<Bar>(name + ":loadingBar", Anchor {
        AnchorType::CenterBottom,
        glm::vec2(0, 5),
        1
    }, 0.6f, 1.5f);

    text = world->InstantiateUIObject<Text>(name + ":statusText", "font", Anchor {
        AnchorType::CenterBottom,
        glm::vec2(0, 15),
        0.3f
    });
}

void LoadingBar::Render()
{
    std::string str;
    bar->SetFillAmount(statusFunc(&str));
    text->SetText(str);
}
