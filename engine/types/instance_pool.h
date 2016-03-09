#pragma once
#include <vector>
#include <map>

/// <summary>
/// Stores all instaces of the inheriting class
/// </summary>
template<typename T>
class InstancePool
{
    public:
        InstancePool();
        void Priority(const int priority);
        T &GetInstance(const int id);
    protected:
        static std::vector<T *> instances;
        static std::map<int, int> location;
    private:
        unsigned int instanceId;
        unsigned int priorityIndex;
};

template<typename T>
std::map<int, int> InstancePool<T>::location;

template<typename T>
std::vector<T *> InstancePool<T>::instances;

template <typename T>
InstancePool<T>::InstancePool()
{
    instanceId = static_cast<unsigned int>(instances.size());
    priorityIndex = instanceId;
    instances.push_back(static_cast<T *>(this));
    location[instanceId] = priorityIndex;
}

template <typename T>
void InstancePool<T>::Priority(const int priority)
{
    // keep reference
    auto &instance = instances[priorityIndex];
    // remove from position
    instances.erase(instances.begin() + priorityIndex);
    // move to new place
    instances.insert(priority, instance);
    // update location
    location[instanceId] = priority;
}

template <typename T>
T &InstancePool<T>::GetInstance(const int id)
{
    return instances[location[id]];
}
