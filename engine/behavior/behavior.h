#pragma once

#include "../core/engine_base.h"
#include "../types/instance_pool.h"

class Behavior : InstancePool<Behavior>
{
    public:
        Behavior();
        virtual ~Behavior();
    protected:
        virtual void Update() = 0;
    private:
        friend void EngineBase::MainLoop();
        static void UpdateAll();
};

