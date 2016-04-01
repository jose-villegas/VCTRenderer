#pragma once

#include "../../../core/interface.h"

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
        static bool drawFramebuffers;
        static bool drawShadowOptions;
        UIMainMenu();
        ~UIMainMenu() override;
};
