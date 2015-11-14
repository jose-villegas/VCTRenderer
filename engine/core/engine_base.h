#pragma once
#include "render_window.h"
#include "interface\vct_interface.h"
#include "engine_assets.h"
#include "deferred_renderer.h"

class EngineBase
{
    public:
        virtual ~EngineBase();
        static std::shared_ptr<EngineBase> &Instance();
        void MainLoop();

        UI &GetUI() { return userInterface; }
        EngineAssets &GetAssets() { return *assetLoader; }
        RenderWindow &GetRenderWindow() { return renderWindow; }
        DeferredRenderer &GetRenderer() { return *renderer; }
    private:
        // over frame user interface
        UI userInterface;
        // glfw window containing ogl context
        RenderWindow renderWindow;
        // loads all deferred shaders file and handles deferred rendering - context dependant
        std::unique_ptr<DeferredRenderer> renderer;
        // loads all scene raw data and uploads to gpu - context dependant
        std::unique_ptr<EngineAssets> assetLoader;
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineBase();
        EngineBase(EngineBase const &r);
        EngineBase(EngineBase const &&r);
        EngineBase &operator=(EngineBase const &r);
        // imports assets and initializes engine libraries
        void Initialize();
};
