#pragma once
#include <chrono>
#include <map>

class IMetric
{
};

template<typename T>
class Metric : IMetric
{
    public:
        T value;
        explicit Metric(const T &value);
};

class ElapsedTime : public Metric<float>
{
    public:
        void Begin();
        void End();
        explicit ElapsedTime(const float &value);
    private:
        std::chrono::nanoseconds time[2];
};

class MovingAverage : public Metric<float>
{
    public:
        void Begin();
        void End();
        void Reset();
        explicit MovingAverage();
    private:
        float sum;
        long long count;
        std::chrono::nanoseconds time[2];
};

class MetricsReport
{
    public:
        template<typename T>
        void Write(const std::string id, const Metric<T> &value);
        template<typename T>
        const T &Read(const std::string id);

        MetricsReport() = default;
        ~MetricsReport() = default;
    private:
        static std::map<std::string, IMetric *> storage;
};

template <typename T>
Metric<T>::Metric(const T &value)
{
    this->value = value;
}

template <typename T>
void MetricsReport::Write(const std::string id, const Metric<T> &value)
{
    storage[id] = &value;
}

template <typename T>
const T &MetricsReport::Read(const std::string id)
{
    static_assert(storage.find(id) != storage.end(), "Id not found");
    return static_cast<Metric<T> *>(storage[id])->value;
}
