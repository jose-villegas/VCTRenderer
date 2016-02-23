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
        static void ResetActive();
        /// <summary>
        /// Sets this instance as active.
        /// </summary>
        virtual void SetAsActive();
        /// <summary>
        /// Return whether this instance is the one marked as active.
        /// </summary>
        /// <returns></returns>
        bool IsActive();
        /// <summary>
        /// Returns the active instance;
        /// </summary>
        /// <returns></returns>
        static std::unique_ptr<T> &Active();
    protected:
        /// <summary>
        /// The current instance marked as active
        /// </summary>
        static std::unique_ptr<T> current;
        /// <summary>
        /// This control boolean determines if SetAsActive has been called.
        /// Its value has to be changed by inheriting classes logic.
        /// </summary>
        static bool changedActive;
        ~SingleActive();
};

template<class T>
bool SingleActive<T>::changedActive = true;
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

template <class T>
void SingleActive<T>::ResetActive()
{
    current.release();
    current.reset(static_cast<T *>(nullptr));
    changedActive = true;;
}

template<class T>
void SingleActive<T>::SetAsActive()
{
    current.release();
    current.reset(static_cast<T *>(this));
    changedActive = true;
}

template <class T>
bool SingleActive<T>::IsActive()
{
    return current.get() == static_cast<T *>(this);
}

template<class T>
std::unique_ptr<T> &SingleActive<T>::Active()
{
    return current;
}
