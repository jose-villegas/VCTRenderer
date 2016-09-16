#pragma once

#include "../core/engine_base.h"
#include "../types/instance_pool.h"

/// <summary>
/// Logic implemented in the <see cref="Update"/>
/// method is meant to be implemented by all inheriting classes.
/// <see cref="UpdateAll"/> will call all implementations of
/// <see cref="Update"/> from instances of classes inheriting
/// from <see cref="Behavior"/>
/// </summary>
/// <seealso cref="InstancePool{Behavior}" />
class Behavior : public InstancePool<Behavior>
{
    public:
        Behavior();
        virtual ~Behavior();
    protected:
        /// <summary>
        /// Called per frame, contains the behavior's logic
        /// </summary>
        virtual void Update() = 0;
    private:
        friend void EngineBase::MainLoop() const;
        static void UpdateAll();
};

