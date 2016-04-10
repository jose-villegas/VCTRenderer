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
        virtual ~InstancePool();
        void Priority(const long long priority);
        T &GetInstance(const long long id);
        InstancePool<T> &operator=(const InstancePool<T> &rhs);
    protected:
        static std::vector<T *> instances;
        static std::map<long long, long long> location;
    private:
        unsigned long long instanceId;
        unsigned long long priorityIndex;
};

template<typename T>
std::map<long long, long long> InstancePool<T>::location;

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
InstancePool<T>::~InstancePool()
{
    // delete self from collections
    auto index = location[instanceId];
    location.erase(instanceId);
    instances.erase(instances.begin() + index);

    // update locations
    for (size_t i = 0; i < instances.size(); ++i)
    {
        location[instances[i]->instanceId] = i;
    }
}

template <typename T>
void InstancePool<T>::Priority(const long long priority)
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
T &InstancePool<T>::GetInstance(const long long id)
{
    return instances[location[id]];
}

template <typename T>
InstancePool<T> &InstancePool<T>::operator=(const InstancePool<T> &rhs)
{
    return instanceId == rhs.instanceId;
}