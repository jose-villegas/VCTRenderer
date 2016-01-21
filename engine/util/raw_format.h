#pragma once
#include <stack>
#include <vector>
#include <typeinfo>

class RawFormat
{
    public:
        template<typename T>
        struct DataSegment
        {
            T * pointer;
            size_t size;
            size_t offset;

            explicit DataSegment(T * pointer);
            DataSegment(T * pointer, size_t size);
            DataSegment(T * pointer, size_t size, size_t offset);
        };

        template<typename T, typename... Args>
        void Stack(DataSegment<T> segment, Args... args);
        template<typename T>
        void Stack(DataSegment<T> segment);

        virtual void BuildFormat() = 0;
        void * RawData();

        template<typename T>
        RawFormat &operator<<(const DataSegment<T> &segment);

        RawFormat();
    protected:
        ~RawFormat();
    private:
        void * rawDataPointer;
        size_t wholeSize;
        std::vector<DataSegment<void>> format;
};

template <typename T>
RawFormat::DataSegment<T>::DataSegment(T * pointer)
{
    this->pointer = pointer;
    this->size = sizeof(T);
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(T * pointer, size_t size)
{
    this->pointer = pointer;
    this->size = size;
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(T * pointer, size_t size, size_t offset)
{
    this->pointer = pointer;
    this->size = size;
    this->offset = offset;
}

template <typename T, typename ... Args>
void RawFormat::Stack(DataSegment<T> segment, Args... args)
{
    Stack(segment);
    Stack(args);
}

template <typename T>
void RawFormat::Stack(DataSegment<T> segment)
{
    format.push_back(
        DataSegment<void>
        (
            static_cast<void *>(segment.pointer),
            segment.size,
            segment.offset
        )
    );
}

template <typename T>
RawFormat &RawFormat::operator<<(const DataSegment<T> &segment)
{
    Stack(segment);
    return *this;
}
