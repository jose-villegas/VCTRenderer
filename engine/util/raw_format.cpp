#include "raw_format.h"

void * RawFormat::BuildRawData()
{
    auto data = reinterpret_cast<unsigned char *>(malloc(wholeSize));
    auto begin = data;

    while (!format.empty())
    {
        auto &segment = format.front();
        // transfer data
        data += segment.offset;

        if (segment.pointer != nullptr)
        {
            memcpy(data, segment.pointer, segment.size);
        }

        segment.pointer = data;
        data += segment.size;
        // save to hash map
        accessMap[segment.name] = segment;
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

RawFormat::RawFormat() : _isBuilt(false), rawDataPointer(nullptr), wholeSize(0)
{
}

void RawFormat::Build()
{
    if (!_isBuilt)
    {
        StackFormat();
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
