#pragma once

#include "../../../interface/interface.h"

class UISceneCameras : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneCameras();
        ~UISceneCameras();
};

