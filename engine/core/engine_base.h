#pragma once
#include "render_window.h"
#include "interface\vct_interface.h"
#include "engine_assets.h"
#include "deferred_renderer.h"

class EngineBase
{
    public:
        virtual ~EngineBase();

        void MainLoop();
        static std::shared_ptr<EngineBase> &Instance();

        static UI &Interface() { return Instance()->userInterface; }
        static EngineAssets &Assets() { return *Instance()->assetLoader; }
        static RenderWindow &Window() { return Instance()->renderWindow; }
        static DeferredRenderer &Renderer() { return *Instance()->renderer; }
    private:
        // over frame user interface
        UI userInterface;
        // glfw window containing ogl context
        RenderWindow renderWindow;
        // loads all deferred shaders file and handles
        // deferred rendering - context dependant
        std::unique_ptr<DeferredRenderer> renderer;
        // loads all scene raw data and uploads
        // to gpu - context dependant
        std::unique_ptr<EngineAssets> assetLoader;
        // imports assets and initializes engine libraries
        void Initialize();
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineBase();
        EngineBase(EngineBase const &r);
        EngineBase(EngineBase const &&r);
        EngineBase &operator=(EngineBase const &r);
};
