#ifndef LOADING_BAR_H
#define LOADING_BAR_H

#include "Engine/UI/Bar.hpp"
#include "Engine/UI/UIObject.hpp"

#include <functional>

class LoadingBar : public UIObject
{
    public:
        LoadingBar(World* world, std::string name, std::function<float(std::string* status)> statusFunc);
        
        void Render() override;

    private:
        std::function<float(std::string* status)> statusFunc;

        std::shared_ptr<Bar> bar;
        std::shared_ptr<Text> text;
};

#endif
