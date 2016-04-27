#pragma once

#include "../core/interface.h"

class UIFramerate : public Interface
{
    protected:
        void Draw() override;
    public:
        UIFramerate();
        ~UIFramerate();
};

