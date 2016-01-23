#pragma once
#include <unordered_map>
#include <queue>

class RawFormat
{
    public:
        template<typename T>
        struct DataSegment
        {
            std::string name;
            T * pointer;
            size_t size;
            size_t offset;

            explicit DataSegment(std::string name);
            DataSegment(std::string name, size_t size);
            DataSegment(std::string name, size_t size, size_t offset);

            explicit DataSegment(std::string name, T * pointer);
            DataSegment(std::string name, T * pointer, size_t size);
            DataSegment(std::string name, T * pointer, size_t size, size_t offset);
        };

        template<typename T, typename... Args>
        void Stack(DataSegment<T> segment, Args... args);
        template<typename T>
        void Stack(DataSegment<T> segment);

        void Build();
        void * RawData();

        template<typename T>
        RawFormat &operator<<(const DataSegment<T> &segment);

        RawFormat();
    protected:
        virtual void StackFormat() = 0;
        template<typename T>
        T * SegmentPointer(std::string name);
        ~RawFormat();
    private:
        bool _isBuilt;
        void * rawDataPointer;
        size_t wholeSize;
        std::queue<DataSegment<void>> format;
        std::unordered_map<std::string, DataSegment<void>> accessMap;

        void * BuildRawData();
};

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name, T * pointer)
{
    this->name = name;
    this->pointer = pointer;
    this->size = sizeof(T);
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name, T * pointer,
                                       size_t size)
{
    this->name = name;
    this->pointer = pointer;
    this->size = size;
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name, T * pointer,
                                       size_t size, size_t offset)
{
    this->name = name;
    this->pointer = pointer;
    this->size = size;
    this->offset = offset;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name)
{
    this->name = name;
    this->pointer = nullptr;
    this->size = sizeof(T);
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name, size_t size)
{
    this->name = name;
    this->pointer = nullptr;
    this->size = size;
    this->offset = 0;
}

template <typename T>
RawFormat::DataSegment<T>::DataSegment(std::string name, size_t size,
                                       size_t offset)
{
    this->name = name;
    this->pointer = nullptr;
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
    if (_isBuilt)
    {
        return;
    }

    format.push(
        DataSegment<void>
        (
            static_cast<void *>(segment.pointer),
            segment.size,
            segment.offset
        )
    );
    wholeSize += segment.size + segment.offset;
}

template <typename T>
RawFormat &RawFormat::operator<<(const DataSegment<T> &segment)
{
    Stack(segment);
    return *this;
}

template <typename T>
T * RawFormat::SegmentPointer(std::string name)
{
    if (accessMap.find(name) != accessMap.end())
    {
        return reinterpret_cast<T *>(accessMap[name].pointer);
    }

    return nullptr;
}
