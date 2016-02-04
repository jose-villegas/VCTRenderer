#include "raw_format.h"
#include "data_segment.h"

void * RawFormat::BuildRawData()
{
    if (rawDataPointer != nullptr)
    {
        free(rawDataPointer);
    }

    auto data = reinterpret_cast<unsigned char *>(malloc(wholeSize));
    auto begin = data;

    while (!format.empty())
    {
        auto segment = static_cast<DataSegment<unsigned char> *>(format.front());
        // transfer data
        data += segment->offset;

        if (segment->pointer != nullptr)
        {
            memcpy(data, segment->pointer, segment->size);
            free(segment->pointer);
        }

        segment->pointer = data;
        data += segment->size;
        // pop format stack
        format.pop();
    }

    // return whole pointer
    return rawDataPointer = begin;
}

void * RawFormat::RawData()
{
    if (rawDataPointer != nullptr)
    {
        return rawDataPointer;
    }

    // build whole data
    Build();
    // return built raw pointer
    return rawDataPointer;
}

RawFormat::RawFormat() : isBuilt(false), rawDataPointer(nullptr), wholeSize(0)
{
}

void RawFormat::Build()
{
    if (!isBuilt)
    {
        rawDataPointer = BuildRawData();
        // can't enter this function anymore
        // once whole data pointer is built
        isBuilt = true;
    }
}

RawFormat::~RawFormat()
{
    if (rawDataPointer != nullptr)
    {
        free(rawDataPointer);
    }

    rawDataPointer = nullptr;
}

void RawFormat::SegmentPush(Segment * segment)
{
    if (isBuilt)
    {
        throw std::logic_error(
            "RawFormat is already built, can't push more data segments"
        );
    }

    // build format queue
    format.push(segment);
    wholeSize += segment->size + segment->offset;
}
