#pragma once
#include <memory>

/// <summary>
/// Template class for set up a single instance of
/// certain class as active, differentiating it
/// from the rest.
/// </summary>
template<class T> class SingleActive
{
    public:
        ~SingleActive();
        /// <summary>
        /// Sets this instance as active.
        /// </summary>
        virtual void SetAsActive();
        /// <summary>
        /// Returns the active instance;
        /// </summary>
        /// <returns></returns>
        static std::unique_ptr<T> &Active();
    protected:
        static std::unique_ptr<T> current;
};

/// <summary>
/// The current active instance
/// </summary>
template<class T>
std::unique_ptr<T> SingleActive<T>::current = nullptr;

template<class T>
SingleActive<T>::~SingleActive()
{
    // release ownership but don't delete
    if (static_cast<T *>(this) == current.get())
    {
        current.release();
    }
}

template<class T>
void SingleActive<T>::SetAsActive()
{
    current.release();
    current.reset(static_cast<T *>(this));
}

template<class T>
std::unique_ptr<T> &SingleActive<T>::Active()
{
    return current;
}
