#pragma once

#include "interface_renderer.h"
#include "../core/engine_base.h"
#include "../types/instance_pool.h"

/// <summary>
/// Holds interfaces code it the <see cref="Draw"/>
/// method to be executed by all inheriting classes
/// during rendering.
/// </summary>
/// <seealso cref="InterfaceRenderer" />
/// <seealso cref="InstancePool{Interface}" />
class Interface : InterfaceRenderer, InstancePool<Interface>
{
    public:
        Interface();
        virtual ~Interface();
    protected:
        ImGuiIO * io;
        ImGuiStyle * style;
        // application specific UI code
        virtual void Draw() = 0;
    private:
        friend void EngineBase::MainLoop();
        static void DrawAll();
};
