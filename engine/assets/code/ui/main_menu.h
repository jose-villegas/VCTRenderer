#pragma once

#include "../../../interface/interface.h"

class EngineBase;

class UIMainMenu : public Interface
{
    protected:
        void Draw() override;
    public:
        static bool drawSceneLoader;
        static bool drawFramerate;
        static bool drawSceneCameras;
        static bool drawSceneLights;
        UIMainMenu();
        ~UIMainMenu() override;
};
