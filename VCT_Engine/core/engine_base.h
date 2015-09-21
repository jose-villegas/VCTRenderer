#pragma once
#include "interface\render_window.h"
#include "misc\External_initializer.h"
#include "interface\vct_interface.h"
#include "engine_assets.h"
#include "deferred_handler.h"
#include "renderer.h"

class ExecutionInfo
{
    public:
        int activeScene;
        int activeCamera;

        virtual ~ExecutionInfo() {};
        ExecutionInfo() : activeScene(0), activeCamera(0) {};
};

class EngineBase
{
    private:
        // over frame user interface
        UI userInterface;
        // glfw window containing ogl context
        RenderWindow renderWindow;
        // initializes external libraries and prints their info
        ExternalInitializer initializer;
        // loads all deferred shaders file and handles deferred rendering - context dependant
        std::unique_ptr<Renderer> renderer;
        // loads all scene raw data and uploads to gpu - context dependant
        std::unique_ptr<EngineAssets> assetLoader;
        // No copying, copy, move assignment allowed of this class or any derived class
        EngineBase();
        EngineBase(EngineBase const &r);
        EngineBase & operator=(EngineBase const &r);
        EngineBase(EngineBase const &&r);
        // imports assets and initializes engine libraries
        void Initialize();
        // contains information about application runtime
        ExecutionInfo execInfo;
    public:
        virtual ~EngineBase();

        static std::shared_ptr<EngineBase> &Instance();
        // returns interface handler
        UI &GetUI() { return userInterface; }
        // returns context active rendering window
        RenderWindow &GetRenderWindow() { return renderWindow; }
        // rendering main loop
        void MainLoop();
        // returns the engine assets handler class
        EngineAssets &GetAssets() { return *assetLoader; }
        // runtime base engine info
        ExecutionInfo &GetExecInfo() { return execInfo; }
        // renderer contains necesarry shaders and state handling for deferred rendering
        Renderer &GetRenderer() { return *renderer; }
};
