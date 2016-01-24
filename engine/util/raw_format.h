#pragma once
#include <memory>
#include <unordered_map>
#include <queue>
#include "data_segment.h"

class RawFormat
{
    public:
        template<typename T, size_t _Size = sizeof(T)>
        using RType = std::unique_ptr<DataSegment<T, _Size>>;

        template<typename T, size_t _Size = sizeof(T)>
        DataSegment<T, _Size> * SegmentPush(std::string name);
        template<typename T, size_t _Size = sizeof(T)>
        DataSegment<T, _Size> * SegmentPush(std::string name, size_t offset);
        template<typename T, size_t _Size = sizeof(T)>
        DataSegment<T, _Size> * SegmentPush(std::string name, T * pointer);
        template<typename T, size_t _Size = sizeof(T)>
        DataSegment<T, _Size> * SegmentPush(std::string name, T * pointer,
                                            size_t offset);
        /// <summary>
        /// Builds the raw pointer and the hash map.
        /// this function can only be called once
        /// </summary>
        void Build();
        /// <summary>
        /// Returns the raw data pointer. Will call Build
        /// the raw format hasn't been built already
        /// </summary>
        /// <returns>Pointer holding all the stacked data</returns>
        void * RawData();
        /// <summary>
        /// Will free previous stored data, hash map and format queue
        /// enables Build to be called again. <see cref="Push"> calls
        /// need to be redone for rebuilding the queue format.
        /// </summary>
        void SetForRebuild();

        RawFormat();
    protected:
        template<typename T>
        T * SegmentPointer(std::string name);
        ~RawFormat();
    private:
        bool _isBuilt;
        void * rawDataPointer;
        size_t wholeSize;
        std::queue<Segment *> format;
        std::unordered_map<std::string, Segment *> accessMap;

        void * BuildRawData();
};

template <typename T, size_t _Size> DataSegment<T, _Size> *
RawFormat::SegmentPush(std::string name)
{
    return SegmentPush<T, _Size>(name, reinterpret_cast<T *>(nullptr), 0);
}

template <typename T, size_t _Size> DataSegment<T, _Size> *
RawFormat::SegmentPush(std::string name, size_t offset)
{
    return SegmentPush<T, _Size>(name, reinterpret_cast<T *>(nullptr), offset);
}

template <typename T, size_t _Size> DataSegment<T, _Size> *
RawFormat::SegmentPush(std::string name, T * pointer)
{
    return SegmentPush<T, _Size>(name, pointer, 0);
}

template <typename T, size_t _Size> DataSegment<T, _Size> *
RawFormat::SegmentPush(std::string name, T * pointer, size_t offset)
{
    if (_isBuilt)
    {
        throw std::logic_error(
            "RawFormat is already built, can't push more data segments"
        );
    }

    // new format segment
    auto segment = new DataSegment<T, _Size>(name, pointer, offset);
    // build format queue
    format.push(segment);
    wholeSize += _Size + offset;
    // return newly created segment
    return segment;
}

template <typename T>
T * RawFormat::SegmentPointer(std::string name)
{
    // build raw pointer if it hasn't been built
    Build();

    // find key name
    if (accessMap.find(name) != accessMap.end())
    {
        return
            reinterpret_cast<T *>
            (
                accessMap[name]->vPointer
            );
    }

    return nullptr;
}
