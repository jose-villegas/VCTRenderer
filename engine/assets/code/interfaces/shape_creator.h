#pragma once
#include "../../../core/interface.h"

class UIShapeCreator : public Interface
{
    protected:
        void Draw() override;
    public:
        UIShapeCreator() = default;
        ~UIShapeCreator() = default;
};