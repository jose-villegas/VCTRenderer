#pragma once

class Segment
{
    public:
        std::string name;
        void * vPointer;
        size_t size;
        size_t offset;

        Segment(const std::string &name, void * pointer, size_t size, size_t offset);
        Segment(Segment &&other) noexcept;
        Segment(const Segment &other) = delete;
        // deleted constructor and assignments
        Segment() = delete;
};

inline Segment::Segment(const std::string &name, void * pointer, size_t size,
                        size_t offset)
{
    this->name = name;
    this->offset = offset;
    this->size = size;
    this->vPointer = pointer;
}

inline Segment::Segment(Segment &&other) noexcept:
    name(move(other.name)),
    vPointer(std::move(other.vPointer)),
    size(std::move(other.size)),
    offset(std::move(other.offset))
{
}

template<typename T, size_t _Size = sizeof(T)>
class DataSegment : public Segment
{
    public:
        T * pointer;

        explicit DataSegment(const std::string &name, T * pointer = nullptr,
                             size_t offset = 0);
        DataSegment(DataSegment &&other) noexcept;
        DataSegment(const DataSegment &other) = delete;
        // deleted constructor and assignments
        DataSegment() = delete;
        DataSegment &operator=(const DataSegment &) = delete;
        // assignments
        T operator=(const T &value);
};

template <typename T, size_t _Size>
DataSegment<T, _Size>::DataSegment(const std::string &name, T * pointer,
                                   size_t offset) :
    Segment
    (
        name,
        static_cast<void *>(pointer),
        _Size,
        offset
    ),
    pointer(pointer)
{
}

template <typename T, size_t _Size>
DataSegment<T, _Size>::DataSegment(DataSegment &&other) noexcept:
    Segment
    (
        std::move(other.name),
        std::move(static_cast<void *>(other.pointer)),
        std::move(_Size),
        std::move(other.offset)
    ),
    pointer(std::move(other.pointer))
{
}

template <typename T, size_t _Size>
T DataSegment<T, _Size>::operator=(const T &value)
{
    *this->pointer = value;
    return value;
}
