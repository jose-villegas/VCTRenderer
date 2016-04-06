#pragma once

#include "../../../core/interface.h"

class UIGlobalIllumination : public Interface
{
    protected:
        void Draw() override;
    public:
        UIGlobalIllumination();
        ~UIGlobalIllumination();
};

