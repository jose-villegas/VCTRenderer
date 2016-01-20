#include "raw_format.h"

void RawFormat::Stack(DataSegment segment)
{
    format.push(segment);
}

RawFormat::RawFormat()
{
}


RawFormat::~RawFormat()
{
}

void RawFormat::operator<<(const DataSegment &segment)
{
    Stack(segment);
}
