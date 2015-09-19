// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "core\base.h"

// debug mode controller
//#define VCT_ENGINE_LOW_PROFILE

int main(int argc, char* argv[])
{
    using namespace VCT_ENGINE;
    #ifdef VCT_ENGINE_LOW_PROFILE // DEBUG
    // pause in positive case
    bool errorCaptured = false;

    try
    {
        // instance engine core to load all assets and
        // relevant data and start rendering main loop
        Base::Instance()->MainLoop();
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

    #else // RELEASE
    // instance engine core to load all assets and
    // relevant data and start rendering main loop
    Base::Instance()->MainLoop();
    #endif
    // exit application
    return 0;
}

