#include "CountdownText.hpp"
#include "Engine/UI/Text.hpp"
#include "Engine/World.hpp"
#include <string>

CountdownText::CountdownText(World* world, std::string name, float dur) : UIObject(world, name), dur(dur / 3.f)
{
    txt = world->InstantiateUIObject<Text>("countdown_txt", "font", Anchor {
        AnchorType::Center,
        glm::vec2(0, 0),
        1
    })
    ->SetText("3")
    ->SetColor(glm::vec3(0.95f, 0.22f, 0.03f));

    timer.Start();
    cnt = 3;
    scale = 1.f;
    alpha = 1.f;
}

void CountdownText::Tick(float deltaTime)
{
    scale += deltaTime * 0.2f;
    txt->SetAnchor(Anchor {
        AnchorType::Center,
        glm::vec2(0, 0),
        scale
    });

    alpha -= deltaTime * 0.3f;

    txt->SetColor(glm::vec4(0.95f, 0.22f, 0.03f, alpha));

    if(timer.TimeSinceStart() >= dur)
    {
        cnt--;
        if(cnt > 0) txt->SetText(std::to_string(cnt));
        else if(cnt == 0) txt->SetText("GO");
        else txt->SetText("");

        timer.Start();
        scale = 1.f;
        alpha = 1.f;
    }
}
