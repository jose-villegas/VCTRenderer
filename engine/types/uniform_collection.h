#pragma once
#include <vector>

/// <summary>
/// Holds collections of program uniforms of the same type
/// these collections have to be identifier by countable identifier
/// </summary>
template<typename T1, typename T2>
class UniformCollection
{
    public:
        /// <summary>
        /// Active uniforms identifiers
        /// </summary>
        /// <returns>A vector containing all active identifiers</returns>
        const std::vector<T2> &Actives() const;
        /// <summary>
        /// Resizes the specified identifier vector
        /// </summary>
        /// <param name="idCount">The identifier count.</param>
        void Resize(const size_t idCount);
        /// <summary>
        /// Saves the specified identifier and related program uniform
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <param name="uniform">The program uniform.</param>
        void Save(T2 id, T1 uniform);
        /// <summary>
        /// Determines whether the class holds an uniform
        /// associated with the specified identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>boolean</returns>
        bool Has(const T2 &id) const;
        /// <summary>
        /// Indexes for the class, returns the id associated uniform
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>The uniform</returns>
        T1 &operator[](const T2 &id);
    private:
        std::vector<std::pair<T2, T1> *> links;
        std::vector<T2> actives;
};


template<typename T1, typename T2>
void UniformCollection<T1, T2>::Resize(const size_t idCount)
{
    links.clear();
    links.resize(idCount);
}

template<typename T1, typename T2>
void UniformCollection<T1, T2>::Save(T2 id, T1 uniform)
{
    if (static_cast<int>(id) < 0 || static_cast<size_t>(id) >= links.size())
    {
        return;
    }

    actives.push_back(std::move(id));
    links[static_cast<int>(id)] = new std::pair<T2, T1>(actives.back(),
            std::move(uniform));
}

template<typename T1, typename T2>
bool UniformCollection<T1, T2>::Has(const T2 &id) const
{
    return links[static_cast<int>(id)] != nullptr;
}

template<typename T1, typename T2>
const std::vector<T2> &UniformCollection<T1, T2>::Actives()
const
{
    return actives;
}

template<typename T1, typename T2>
T1 &UniformCollection<T1, T2>::operator[](const T2 &id)
{
    return links[static_cast<int>(id)]->second;
}


