#pragma once
#include "raw_format.h"

class RawFormat::Segment
{
    public:
        size_t size;
        size_t offset;

        Segment(size_t size, size_t offset);
        Segment(const Segment &other) = delete;
        // deleted constructor and assignments
        Segment() = delete;
};

inline RawFormat::Segment::Segment(size_t size, size_t offset)
{
    this->offset = offset;
    this->size = size;
}
