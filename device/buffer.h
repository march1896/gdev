#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
#include <algorithm>

#include "value.h"

namespace Device {

    // A stream used for inter component communication
    class FifoStream
    {
    public:
        typedef StructValueRef Element;

    protected:
        StructType m_structure;

        U8* m_storage;
        U32 m_capacity;

        U32 m_begin;
        U32 m_end;

        friend class StreamBuffer;

    public:
        FifoStream();

        ~FifoStream();

        U32 addChannel(Semantic const& semantic, BuiltinType const& type);

        U32 getChannelIndex(Semantic const& semantic) const;

        U32 numChannels() const;

        void resetChannels();

        void setCapacity(U32 maxNumElements);

        U32 getNumElements() const;

        bool isEmpty() const;

        bool isFull() const;

        Element pushData();

        Element front();

        void popData();
    };

    // This class takes a FifoStream and provide interface to browse its storage as a buffer.
    class StreamBuffer
    {
    public:
        typedef StructValueRef Element;

    protected:
        StructType m_structure;
        U8* m_storage;
        U32 m_length;

    public:
        StreamBuffer() {}

        StreamBuffer(FifoStream const& fifoStream);

        U32 getLength();

        Element getElement(U32 index);

        StructType getElementStruct() const;
    };

} // namespace Device

#endif // _BUFFER_H_
