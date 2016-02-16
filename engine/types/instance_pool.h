#pragma once
#include <vector>

/// <summary>
/// Stores all instaces of the inheriting class
/// </summary>
template<typename T>
class InstancePool
{
    public:
        InstancePool();
    protected:
        static std::vector<T *> instances;
};

template<typename T>
std::vector<T *> InstancePool<T>::instances;

template <typename T>
InstancePool<T>::InstancePool()
{
    instances.push_back(static_cast<T *>(this));
}
