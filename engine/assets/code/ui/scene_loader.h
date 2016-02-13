#pragma once

#include "../../../interface/interface.h"

class EngineBase;

class UISceneLoader : public Interface
{
    public:
        void Draw() override;
        UISceneLoader();
        ~UISceneLoader() override;
    private:
        ImVec2 uiPos, uiSize;
};
