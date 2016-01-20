#pragma once
#include <stack>

class RawFormat
{
    public:
        struct DataSegment
        {
            void * pointer;
            size_t size;
            size_t offset;
        };

        template<typename... Args>
        void Stack(DataSegment segment, Args... args);
        void Stack(DataSegment segment);

        RawFormat();
        ~RawFormat();

        void operator<<(const DataSegment &segment);
    private:
        std::stack<DataSegment> format;
};

template <typename ... Args>
void RawFormat::Stack(DataSegment segment, Args... args)
{
    Stack(segment);
    Stack(args);
}
