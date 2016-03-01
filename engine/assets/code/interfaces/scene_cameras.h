#pragma once

#include "../../../core/interface.h"

class UISceneCameras : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneCameras();
        ~UISceneCameras();
};

