#pragma once

#include "interface_renderer.h"
#include "../core/engine_base.h"
#include "../types/instance_pool.h"

class Interface : public InterfaceRenderer, InstancePool<Interface>
{
    public:
        Interface();
        virtual ~Interface();
    protected:
        ImGuiIO &io;
        // application specific UI code
        virtual void Draw() = 0;
    private:
        friend void EngineBase::MainLoop();
        static void DrawAll();
};
