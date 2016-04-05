#pragma once

#include "../../../core/interface.h"

class Scene;
class Light;

class UISceneLights : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneLights();
        ~UISceneLights();
};

