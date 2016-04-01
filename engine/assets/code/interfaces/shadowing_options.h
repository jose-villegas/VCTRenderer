#pragma once

#include "../../../core/interface.h"

class UIShadowingOptions : public Interface
{
    protected:
        void Draw() override;
    public:
        UIShadowingOptions();
        ~UIShadowingOptions();
};

