#pragma once
#include <string>

class RawFormat;

class Segment
{
    public:
        std::string name;
        void * vPointer;
        size_t size;
        size_t offset;

        Segment(const std::string &name, void * pointer, size_t size, size_t offset);
        Segment(const Segment &other) = delete;
        // deleted constructor and assignments
        Segment() = delete;
    protected:
        void PushCallback(RawFormat * format);
};

template<typename T, size_t _Size = sizeof(T)>
class DataSegment : public Segment
{
    public:
        T * pointer;
        RawFormat * holder;

        DataSegment(RawFormat * format, const std::string &name,
                    T * pointer = nullptr, size_t offset = 0);
        DataSegment(const DataSegment &other) = delete;
        // deleted constructor and assignments
        DataSegment() = delete;
        DataSegment &operator=(const DataSegment &) = delete;
        // assignments
        T operator=(const T &value);
        const T * operator=(const T values[]);
        void operator=(const std::array < T, _Size / sizeof(T) > &values);
};

template <typename T, size_t _Size>
DataSegment<T, _Size>::DataSegment(RawFormat * from, const std::string &name,
                                   T * pointer, size_t offset) :
    Segment
    (
        name,
        static_cast<void *>(pointer),
        _Size,
        offset
    ),
    pointer(pointer), holder(from)
{
    PushCallback(from);
}

template <typename T, size_t _Size>
T DataSegment<T, _Size>::operator=(const T &value)
{
    if (this->pointer == nullptr)
    {
        this->pointer = static_cast<T *>(malloc(_Size));
    }

    *this->pointer = value;
    return value;
}

template <typename T, size_t _Size>
const T * DataSegment<T, _Size>::operator=(const T values[])
{
    if (this->pointer == nullptr)
    {
        this->pointer = static_cast<T *>(malloc(_Size));
    }

    auto count = _Size / sizeof(T);

    for (size_t i = 0; i < count; i++)
    {
        this->pointer[i] = values[i];
    }

    return values;
}

template <typename T, size_t _Size>
void DataSegment<T, _Size>::operator=(
    const std::array < T, _Size / sizeof(T) > &values)
{
    if (this->pointer == nullptr)
    {
        this->pointer = static_cast<T *>(malloc(_Size));
    }

    for (size_t i = 0; i < values.length; i++)
    {
        this->pointer[i] = values[i];
    }
}

inline Segment::Segment(const std::string &name, void * pointer, size_t size,
                        size_t offset)
{
    this->name = name;
    this->offset = offset;
    this->size = size;
    this->vPointer = pointer;
}
