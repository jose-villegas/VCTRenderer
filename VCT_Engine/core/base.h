#pragma once
#include "interface\render_window.h"
#include "misc\External_initializer.h"
#include "interface\vct_interface.h"
#include "assets.h"

namespace VCT_ENGINE
{
    class ExecutionInfo
    {
        public:
            int activeScene;

            virtual ~ExecutionInfo() {};
            ExecutionInfo() : activeScene(-1) {};
    };

    class Base
    {
        private:
            UI userInterface;
            RenderWindow renderWindow;
            ExternalInitializer initializer;

            std::unique_ptr<Assets> assetLoader;
            // No copying, copy, move assignment allowed of this class or any derived class
            Base();
            Base(Base const &r);
            Base & operator=(Base const &r);
            Base(Base const &&r);
            // imports assets and initializes engine libraries
            void Initialize();
            // contains information about application runtime
            ExecutionInfo execInfo;
        public:
            virtual ~Base();

            static std::shared_ptr<Base> &Instance();
            // returns interface handler
            VCT_ENGINE::UI &GetUI() { return userInterface; }
            // returns context active rendering window
            RenderWindow &GetRenderWindow() { return renderWindow; }
            // rendering main loop
            void MainLoop();
            // returns the engine assets handler class
            Assets &GetAssets() { return *assetLoader; }
            // runtime base engine info
            ExecutionInfo &GetExecInfo() { return execInfo; }
    };

}
