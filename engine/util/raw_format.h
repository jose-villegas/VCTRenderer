#pragma once
#include <queue>
#include "data_segment.h"

class RawFormat
{
    public:
        void SegmentPush(Segment * segment);
        /// <summary>
        /// Builds the raw pointer and the hash map.
        /// this function can only be called once
        /// </summary>
        void Build();
        /// <summary>
        /// Returns the raw data pointer. Will call Build
        /// the raw format hasn't been built already
        /// </summary>
        /// <returns>Pointer holding all the stacked data</returns>
        void * RawData();
        /// <summary>
        /// Will free previous stored data, hash map and format queue
        /// enables Build to be called again. <see cref="Push"> calls
        /// need to be redone for rebuilding the queue format.
        /// </summary>
        void SetForRebuild();
        RawFormat();
    protected:
        ~RawFormat();
    private:
        bool _isBuilt;
        void * rawDataPointer;
        size_t wholeSize;
        std::queue<Segment *> format;

        void * BuildRawData();
};
