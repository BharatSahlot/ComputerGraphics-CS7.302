#ifndef COUNTDOWN_TEXT_H
#define COUNTDOWN_TEXT_H

#include "Engine/UI/Text.hpp"
#include "Engine/UI/UIObject.hpp"
#include "Engine/Utils/Timer.hpp"
class CountdownText : public UIObject
{
    public:
        CountdownText(World* world, std::string name, float dur);
        
        void Tick(float deltaTime) override;
        // void Render() override;

    private:
        float dur;
        Text* txt;

        Timer timer;
        int cnt;

        float scale;
        float alpha;
};

#endif
