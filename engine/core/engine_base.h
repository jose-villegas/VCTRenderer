#pragma once
#include <memory>

#include "../interface/vct_interface.h"
#include "render_window.h"

class DeferredRenderer;
class EngineAssets;

class EngineBase
{
    public:
        virtual ~EngineBase();

        void MainLoop();
        static std::shared_ptr<EngineBase> &Instance();

        static UI &Interface() { return Instance()->userInterface; }
        static EngineAssets &Assets() { return *Instance()->assets; }
        static RenderWindow &Window() { return Instance()->renderWindow; }
        static DeferredRenderer &Renderer() { return *Instance()->renderer; }
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineBase(EngineBase const &r) = delete;
        EngineBase(EngineBase const &&r) = delete;
        EngineBase &operator=(EngineBase const &r) = delete;
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
        std::unique_ptr<EngineAssets> assets;
        // imports assets and initializes engine libraries
        void Initialize();

        EngineBase();
};
