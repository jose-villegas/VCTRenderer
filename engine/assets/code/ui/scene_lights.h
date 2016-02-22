#pragma once

#include "../../../interface/interface.h"

class UISceneLights : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneLights();
        ~UISceneLights();
};

