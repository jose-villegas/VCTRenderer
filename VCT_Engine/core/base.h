#pragma once
#include "interface\render_window.h"
#include "misc\External_initializer.h"
#include "interface\vct_interface.h"
#include "asset_loader.h"

namespace VCT_ENGINE
{

    class Base
    {
        private:
            UI userInterface;
            RenderWindow renderWindow;
            ExternalInitializer initializer;
            AssetLoader assetLoader;
            // unique engine core instance
            static Base * coreInstance;
            // No copying, copy, move assignment allowed of this class or any derived class
            Base();
            Base(Base const &);
            Base &operator=(Base const &);
            Base(Base const &&);
        public:
            virtual ~Base();
            static Base * Instance();

            VCT_ENGINE::UI &GetUI() { return userInterface; }
            RenderWindow &GetRenderWindow() { return renderWindow; }
    };

}
