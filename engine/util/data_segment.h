#pragma once
#include "segment.h"
#include "raw_format.h"

#define DATA_SEGMENT_GLM_INTEROP

template<typename T, size_t _Count = 1>
class RawFormat::DataSegment : public Segment
{
    public:
        T * pointer;
        RawFormat * holder;

        explicit DataSegment(RawFormat * format, T * pointer = nullptr,
                             size_t offset = 0);
        // deleted constructor and assignments
        DataSegment(const DataSegment &other) = delete;
        DataSegment() = delete;
        DataSegment &operator=(const DataSegment &) = delete;

        ~DataSegment();
        // assignments
        T operator=(const T &value);
        T * operator=(const std::array <T, _Count> &values);
};

template <typename T, size_t _Count>
RawFormat::DataSegment<T, _Count>::DataSegment(RawFormat * from, T * pointer,
        size_t offset) :
    Segment
    (
        sizeof(T) * _Count,
        offset
    ),
    holder(from)
{
    this->pointer = pointer;
    from->SegmentPush(this);
}

template <typename T, size_t _Count>
RawFormat::DataSegment<T, _Count>::~DataSegment()
{
    if (!holder->isBuilt)
    {
        free(pointer);
        pointer = nullptr;
    }
}

template <typename T, size_t _Count>
T RawFormat::DataSegment<T, _Count>::operator=(const T &value)
{
    if (this->pointer == nullptr)
    {
        this->pointer = static_cast<T *>(malloc(size));
    }

    *this->pointer = value;
    return value;
}

template <typename T, size_t _Count>
T * RawFormat::DataSegment<T, _Count>::operator=(const std::array <T, _Count>
        &values)
{
    if (this->pointer == nullptr)
    {
        this->pointer = static_cast<T *>(malloc(size));
    }

    for (size_t i = 0; i < _Count; i++)
    {
        this->pointer[i] = values[i];
    }

    return pointer;
}

#ifdef DATA_SEGMENT_GLM_INTEROP

#include <glm/detail/type_vec.hpp>
#include <glm/gtc/type_ptr.hpp>

#if (GLM_VERSION_MAJOR > 0) || ( \
    (GLM_VERSION_MINOR > 9) || ( \
        (GLM_VERSION_MINOR == 9) && \
        (GLM_VERSION_PATCH > 5) \
    ))
#define GLM_TPLNS ::glm
#else
#define GLM_TPLNS ::glm::detail
#endif

#define GLM_VEC_UNIFORM_OPS(DIM) \
    template <typename T, glm::precision P> \
    class RawFormat::DataSegment <GLM_TPLNS::tvec##DIM<T, P>> : public Segment \
    { \
        public: \
            T * pointer; \
            RawFormat * holder; \
            \
            explicit DataSegment(RawFormat * from, T * pointer = nullptr, \
                                 size_t offset = 0); \
            \
            DataSegment(const DataSegment &other) = delete; \
            DataSegment() = delete; \
            DataSegment &operator=(const DataSegment &) = delete; \
            \
            ~DataSegment(); \
            GLM_TPLNS::tvec##DIM<T, P> operator=(const GLM_TPLNS::tvec##DIM<T, P> &value); \
    }; \
    template <typename T, glm::precision P> \
    RawFormat::DataSegment<GLM_TPLNS::tvec##DIM<T, P>>::DataSegment(RawFormat * from, \
            T * pointer, size_t offset) : \
                Segment \
                ( \
                  sizeof(GLM_TPLNS::tvec##DIM<T, P>), \
                  offset \
                ), \
                holder(from) \
    { \
        this->pointer = pointer; \
        from->SegmentPush(this); \
    } \
    template <typename T, glm::precision P> \
    RawFormat::DataSegment<GLM_TPLNS::tvec##DIM<T, P>>::~DataSegment() \
    { \
        if (!holder->isBuilt) \
        { \
            free(pointer); \
            pointer = nullptr; \
        } \
    } \
    template <typename T, glm::precision P> \
    GLM_TPLNS::tvec##DIM<T, P> RawFormat::DataSegment<GLM_TPLNS::tvec##DIM<T, P>>::operator= \
            (const GLM_TPLNS::tvec##DIM<T, P> &value) \
    { \
        if (this->pointer == nullptr) \
        { \
            this->pointer = static_cast<T *>(malloc(size)); \
        } \
        \
        memccpy(pointer, GLM_TPLNS::value_ptr(value), size); \
        return value; \
    } \

    GLM_VEC_UNIFORM_OPS(2)
    GLM_VEC_UNIFORM_OPS(3)
    GLM_VEC_UNIFORM_OPS(4)

#undef GLM_VEC_UNIFORM_OPS

#define GLM_MAT_UNIFORM_OPS(R, C) \
    template <typename T, glm::precision P> \
    class RawFormat::DataSegment <GLM_TPLNS::tmat##C##x##R<T, P>> : public Segment \
    { \
        public: \
            T * pointer; \
            RawFormat * holder; \
            \
            explicit DataSegment(RawFormat * from, T * pointer = nullptr, \
                                 size_t offset = 0); \
            \
            DataSegment(const DataSegment &other) = delete; \
            DataSegment() = delete; \
            DataSegment &operator=(const DataSegment &) = delete; \
            \
            ~DataSegment(); \
            GLM_TPLNS::tmat##C##x##R<T, P> operator=(const GLM_TPLNS::tmat##C##x##R<T, P> &value); \
    }; \
    template <typename T, glm::precision P> \
    RawFormat::DataSegment<GLM_TPLNS::tmat##C##x##R<T, P>>::DataSegment(RawFormat * from, \
            T * pointer, size_t offset) : \
                Segment \
                ( \
                  sizeof(GLM_TPLNS::tmat##C##x##R<T, P>), \
                  offset \
                ), \
                holder(from) \
    { \
        this->pointer = pointer; \
        from->SegmentPush(this); \
    } \
    template <typename T, glm::precision P> \
    RawFormat::DataSegment<GLM_TPLNS::tmat##C##x##R<T, P>>::~DataSegment() \
    { \
        if (!holder->isBuilt) \
        { \
            free(pointer); \
            pointer = nullptr; \
        } \
    } \
    template <typename T, glm::precision P> \
    GLM_TPLNS::tmat##C##x##R<T, P> RawFormat::DataSegment<GLM_TPLNS::tmat##C##x##R<T, P>>::operator= \
            (const GLM_TPLNS::tmat##C##x##R<T, P> &value) \
    { \
        if (this->pointer == nullptr) \
        { \
            this->pointer = static_cast<T *>(malloc(size)); \
        } \
        \
        memccpy(pointer, GLM_TPLNS::value_ptr(value), size); \
        return value; \
    } \

    GLM_MAT_UNIFORM_OPS(2, 2)
    GLM_MAT_UNIFORM_OPS(2, 3)
    GLM_MAT_UNIFORM_OPS(2, 4)
    GLM_MAT_UNIFORM_OPS(3, 2)
    GLM_MAT_UNIFORM_OPS(3, 3)
    GLM_MAT_UNIFORM_OPS(3, 4)
    GLM_MAT_UNIFORM_OPS(4, 2)
    GLM_MAT_UNIFORM_OPS(4, 3)
    GLM_MAT_UNIFORM_OPS(4, 4)

#undef GLM_MAT_UNIFORM_OPS

#undef GLM_TPLNS
#endif