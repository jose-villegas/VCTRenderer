#pragma once

#include "../../../core/interface.h"

class UIGeometryBuffer : public Interface
{
    protected:
        void Draw() override;
    public:
        UIGeometryBuffer();
        ~UIGeometryBuffer();
};

