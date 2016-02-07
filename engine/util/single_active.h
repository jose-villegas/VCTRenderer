#pragma once
#include <memory>

template<class T> class SingleActive
{
    public:
        ~SingleActive();
        //static void Active(T &active);
        void SetAsActive();
        static std::unique_ptr<T> &Active();
    protected:
        static std::unique_ptr<T> current;
};

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
