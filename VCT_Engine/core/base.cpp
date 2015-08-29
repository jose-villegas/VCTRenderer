#include "stdafx.h"
#include "base.h"
using namespace VCT_ENGINE;
Base * VCT_ENGINE::Base::coreInstance = nullptr;

// initializes base engine assets and libs
Base::Base()
{
    // initialize external dependencies
    initializer.Initialize();
    // open window and set rendering context
    renderWindow.Open();
    renderWindow.SetAsCurrentContext();
    // initialize context dependant external libs
    initializer.InitializeContextDependant();
    // set interface to current renderwindow
    userInterface.Initialize(renderWindow);
    // load engine demo scene assets
    assetLoader.LoadDemoScenes();
}


Base::~Base()
{
}

Base * VCT_ENGINE::Base::Instance()
{
    if(!coreInstance)
    {
        return coreInstance = new Base();
    }

    return coreInstance;
}
