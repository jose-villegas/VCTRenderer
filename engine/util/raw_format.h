#pragma once
#include <queue>

class RawFormat
{
    public:
        template<typename T, size_t _Count>
        class DataSegment;
        /// <summary>
        /// Returns the raw data pointer. Will call Build
        /// the raw format hasn't been built already
        /// </summary>
        /// <returns>Pointer holding all the stacked data</returns>
        void * RawData();
        size_t Size() const { return wholeSize; };
        RawFormat();
    protected:
        ~RawFormat();
        /// <summary>
        /// Calls <see cref="BuildRawData">.
        /// This function will only work if
        /// <see cref="_isBuilt"> is false.
        /// </summary>
        void Build();
    private:
        class Segment;
        bool isBuilt;
        void * rawDataPointer;
        size_t wholeSize;
        std::queue<Segment *> format;

        void SegmentPush(Segment * segment);
        void * BuildRawData();
};
