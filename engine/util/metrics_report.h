#pragma once

#include <chrono>
#include <map>
#include <string>

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
        /// <summary>
        /// Initializes the beginning clock time
        /// </summary>
        void Begin();
        /// <summary>
        /// Ends the clock and stores the elapsed time from
        /// begin to end.
        /// </summary>
        template<class _Duration = std::chrono::nanoseconds>
        void End();

        explicit ElapsedTime(const long long &value);
    private:
        std::chrono::time_point<std::chrono::steady_clock> time[2];
};

class MetricsReport
{
    public:
        /// <summary>
        /// Writes the given metric value and maps it to the id.
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <param name="value">The value.</param>
        template<typename T>
        static void Write(const std::string id, const Metric<T> &value);
        /// <summary>
        /// Returns the metric value for the given metrics id
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns></returns>
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

template<class _Duration>
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
