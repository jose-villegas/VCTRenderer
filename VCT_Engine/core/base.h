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
            std::vector<char *> availableScenes;
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
            Assets assetLoader;
            // unique engine core instance
            static Base * coreInstance;
            // No copying, copy, move assignment allowed of this class or any derived class
            Base();
            Base(Base const &);
            Base & operator=(Base const &);
            Base(Base const &&);
            // imports assets and initializes engine libraries
            void Initialize();
        public:
            virtual ~Base();

            // contains information about application runtime
            ExecutionInfo execInfo;

            static Base * Instance();
            // returns interface handler
            VCT_ENGINE::UI & GetUI() { return userInterface; }
            // returns context active rendering window
            RenderWindow &GetRenderWindow() { return renderWindow; }
            // rendering main loop
            void MainLoop();
    };

}
