#pragma once

#include "../types/instance_pool.h"
#include "interface_renderer.h"

class Interface : public InterfaceRenderer, InstancePool<Interface>
{
    public:
        Interface();
        virtual ~Interface();
        static void DrawAll();
    protected:
        ImGuiIO &io;
        // application specific UI code
        virtual void Draw() = 0;
};
