#include "data_segment.h"
#include "raw_format.h"

void Segment::PushCallback(RawFormat * format)
{
    static_cast<RawFormat *>(format)->SegmentPush(this);
}
