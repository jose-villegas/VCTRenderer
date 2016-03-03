#pragma once
#include "../../../core/interface.h"

class UIMetrics : public Interface
{
    protected:
        void Draw() override;
    public:
        UIMetrics();
        ~UIMetrics();
};

