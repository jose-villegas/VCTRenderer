// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "core\base.h"

// debug mode controller
#define VCT_ENGINE_LOW_PROFILE

int main(int argc, char* argv[])
{
    // pause in positive case
    bool errorCaptured = false;
    #ifdef VCT_ENGINE_LOW_PROFILE

    try
    {
        // instance engine core to load all assets and relevant data
        VCT_ENGINE::Base * engineCore = VCT_ENGINE::Base::Instance();
        // start rendering main loop
        engineCore->MainLoop();
    }
    catch(oglplus::ProgramBuildError& pbe)
    {
        std::cerr << pbe.Log() << std::endl;
        errorCaptured = true;
    }
    catch(oglplus::Error& err)
    {
        std::cerr << "Error (in " << err.GLFunc()
                  << "') [" << err.SourceFile()
                  << ":" << err.SourceLine()
                  << "]: " << err.what() << std::endl;
        errorCaptured = true;
    }

    if(errorCaptured) std::cin.get();

    #else // DEBUG
    // instance engine core to load all assets and relevant data
    VCT_ENGINE::Base * engineCore = VCT_ENGINE::Base::Instance();
    // start rendering main loop
    engineCore->MainLoop();
    #endif // RELEASE
    // exit application
    return 0;
}

