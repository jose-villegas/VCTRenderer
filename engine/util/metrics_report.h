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

class ElapsedTime : public Metric<long long>
{
    public:
        void Begin();
        template<class _Duration = std::chrono::nanoseconds>
        void End();
        explicit ElapsedTime(const long long &value);
    private:
        std::chrono::time_point<std::chrono::steady_clock> time[2];
};

class MetricsReport
{
    public:
        template<typename T>
        static void Write(const std::string id, const Metric<T> &value);
        template<typename T>
        static const T &Read(const std::string id);

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

template<class _Duration = std::chrono::nanoseconds>
void ElapsedTime::End()
{
    time[1] = std::chrono::steady_clock::now();
    value = std::chrono::duration_cast<_Duration>(time[1] - time[0]).count();
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
