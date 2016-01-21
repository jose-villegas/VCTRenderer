#include "raw_format.h"

void * RawFormat::RawData()
{
    if (rawDataPointer != nullptr)
    {
        return rawDataPointer;
    }

    // calculate whole size
    for (auto &segment : format)
    {
        wholeSize += segment.size + segment.offset;
    }

    auto data = reinterpret_cast<unsigned char *>(malloc(wholeSize));
    auto begin = data;

    for (auto &segment : format)
    {
        data += segment.offset;
        memcpy(data, segment.pointer, segment.size);
        segment.pointer = data;
        data += segment.size;
    }

    return rawDataPointer = begin;
}

RawFormat::RawFormat() : rawDataPointer(nullptr), wholeSize(0)
{
}


RawFormat::~RawFormat()
{
    free(rawDataPointer);
}
