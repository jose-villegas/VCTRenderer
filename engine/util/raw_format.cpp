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
        auto &segment = format.front();
        // transfer data
        data += segment->offset;

        if (segment->vPointer != nullptr)
        {
            memcpy(data, segment->vPointer, segment->size);
            free(segment->vPointer);
        }

        static_cast<DataSegment <unsigned char> *>
        (segment)->pointer = data;

        segment->vPointer = data;

        data += segment->size;

        // pop format stack
        format.pop();
    }

    return rawDataPointer = begin;
}

void * RawFormat::RawData()
{
    if (rawDataPointer != nullptr)
    {
        return rawDataPointer;
    }

    return BuildRawData();
}

void RawFormat::SetForRebuild()
{
    free(rawDataPointer);

    while (!format.empty()) { format.pop(); }

    rawDataPointer = nullptr;
    wholeSize = 0;
    _isBuilt = false;
}

RawFormat::RawFormat() : _isBuilt(false), rawDataPointer(nullptr), wholeSize(0)
{
}

void RawFormat::Build()
{
    if (!_isBuilt)
    {
        rawDataPointer = RawData();
        // can't enter this function anymore
        // once whole data pointer is built
        _isBuilt = true;
    }
}

RawFormat::~RawFormat()
{
    free(rawDataPointer);
}

void RawFormat::SegmentPush(Segment * segment)
{
    if (_isBuilt)
    {
        throw std::logic_error(
            "RawFormat is already built, can't push more data segments"
        );
    }

    // build format queue
    format.push(segment);
    wholeSize += segment->size + segment->offset;
}
