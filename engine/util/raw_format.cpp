#include "raw_format.h"

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
        }

        static_cast<DataSegment <unsigned char> *>
        (segment)->pointer = data;

        segment->vPointer = data;

        data += segment->size;

        // save to hash map
        accessMap[segment->name] = segment;

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
    accessMap.clear();

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
