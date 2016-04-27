#include "metrics_report.h"

void ElapsedTime::Begin()
{
    time[0] = std::chrono::steady_clock::now().time_since_epoch();
}

void ElapsedTime::End()
{
    time[1] = std::chrono::steady_clock::now().time_since_epoch();
    value = static_cast<float>((time[1] - time[0]).count()) * 0.000001;
}

ElapsedTime::ElapsedTime(const float &value) : Metric<float>(value)
{
}

void MovingAverage::Begin()
{
    time[0] = std::chrono::steady_clock::now().time_since_epoch();
}

void MovingAverage::End()
{
    time[1] = std::chrono::steady_clock::now().time_since_epoch();
    auto d = static_cast<float>((time[1] - time[0]).count()) * 0.000001;
    // new value
    count = count + 1;
    //sum
    sum += d;
    //average
    value = sum / count;
}

void MovingAverage::Reset()
{
    value = 0;
    sum = 0;
    count = 0;
}

MovingAverage::MovingAverage() : Metric<float>(0.0f), sum(0), count(0)
{
}
