#pragma once
#include "raw_format.h"

class RawFormat::Segment
{
    public:
        /// <summary>
        /// The segment size
        /// </summary>
        size_t size;
        /// <summary>
        /// The segment offset
        /// </summary>
        size_t offset;

        Segment(size_t size, size_t offset);
        Segment(const Segment &other) = delete;
        Segment() = delete;
};

inline RawFormat::Segment::Segment(size_t size, size_t offset)
{
    this->offset = offset;
    this->size = size;
}
