#pragma once

#include "../../../interface/interface.h"

class UIGeometryBuffer : public Interface
{
    protected:
        void Draw() override;
    public:
        UIGeometryBuffer();
        ~UIGeometryBuffer();
};

