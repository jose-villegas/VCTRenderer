#pragma once

#include "../../../core/interface.h"
#include "../util/metrics_report.h"

class UIFramerate : public Interface
{
    protected:
        void Draw() override;
    public:
        UIFramerate();
        ~UIFramerate();
};

class UIMetrics : public Interface
{
    protected:
        void Draw() override;
    public:
        static MovingAverage ClearStatic;
        static MovingAverage VoxelizationStatic;
        static MovingAverage ClearDynamic;
        static MovingAverage VoxelizationDynamic;
        static MovingAverage RadianceInjection;
        static MovingAverage MipmappingRadiance;
        static MovingAverage BounceInjection;
        static MovingAverage MipmappingBounce;

        UIMetrics() = default;
        ~UIMetrics() = default;
};

