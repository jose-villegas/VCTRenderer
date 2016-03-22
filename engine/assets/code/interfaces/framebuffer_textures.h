#pragma once

#include "../../../core/interface.h"

class UIFramebuffers : public Interface
{
    protected:
        void Draw() override;
    public:
        UIFramebuffers();
        ~UIFramebuffers();
};

