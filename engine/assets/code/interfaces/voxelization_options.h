#pragma once

#include "../../../core/interface.h"

class UIVoxelizationOptions : public Interface
{
    protected:
        void Draw() override;
    public:
        UIVoxelizationOptions();
        ~UIVoxelizationOptions();
};

