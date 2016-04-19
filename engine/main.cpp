#include "core/engine_base.h"
#include "rendering/render_window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <oglplus/error/program.hpp>
#include <iostream>

int main(int argc, char * argv[])
{
    #ifndef NDEBUG // DEBUG
    // pause in positive case
    auto errorCaptured = false;

    try
    {
        // instance engine core to load all assets and
        // relevant data and start rendering main loop
        EngineBase::Instance()->MainLoop();
    }
    catch (const oglplus::ProgramBuildError &pbe)
    {
        std::cerr << pbe.Log() << std::endl;
        errorCaptured = true;
    }
    catch (const oglplus::Error &err)
    {
        std::cerr << "Error (in " << err.GLFunc()
                  << "') [" << err.SourceFile()
                  << ":" << err.SourceLine()
                  << "]: " << err.what() << std::endl;
        errorCaptured = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        errorCaptured = true;
    }

    if (errorCaptured) { std::cin.get(); }

    #else // RELEASE
    // instance engine core to load all assets and
    // relevant data and start rendering main loop
    EngineBase::Instance()->MainLoop();
    #endif
    EngineBase::Terminate();
    // exit application
    return 0;
}

