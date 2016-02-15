#pragma once

#include "../../../interface/interface.h"

class UIFramerate : public Interface
{
    protected:
        void Draw() override;
    public:
        UIFramerate();
        ~UIFramerate();
};

