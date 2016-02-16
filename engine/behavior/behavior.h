#pragma once

#include "../core/engine_base.h"
#include "../types/instance_pool.h"

/// <summary>
/// Holds logic code in the <see cref="Update"/>
/// method to be executed by all inheriting classes
/// during rendering.
/// </summary>
/// <seealso cref="InstancePool{Behavior}" />
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

