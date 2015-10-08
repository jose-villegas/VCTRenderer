// VCT_Engine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "core\engine_base.h"

#ifdef _VISUAL_LEAK_DETECTOR
    #include "vld.h"
#endif

int main(int argc, char* argv[])
{
    #ifndef NDEBUG // DEBUG
    // pause in positive case
    bool errorCaptured = false;

    try
    {
        // instance engine core to load all assets and
        // relevant data and start rendering main loop
        EngineBase::Instance()->MainLoop();
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
    EngineBase::Instance()->MainLoop();
    #endif
    // exit application
    return 0;
}

