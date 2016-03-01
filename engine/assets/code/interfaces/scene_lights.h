#pragma once

#include "../../../core/interface.h"

class UISceneLights : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneLights();
        ~UISceneLights();
};

