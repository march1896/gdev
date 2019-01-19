#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
#include <algorithm>

#include "vmath.h"
#include "semantic.h"
#include "component.h"

namespace Device {

    // LinearStruct's element is in a contiguous memory layout, quite like C structure type.
    class LinearStruct
    {
    protected:
        // field information
        std::vector<U32> m_fieldOffsets;
        std::vector<Semantic> m_fieldSemantics;
        std::vector<Type> m_fieldTypes;
        U32 m_size;

    protected:
        void updateOffsets();

    public:
        LinearStruct();

        // Returns field index.
        U32 addField(Semantic const& semantic, Type const& type);

        // Returns field index.
        U32 getFieldIndex(Semantic const& semantic) const;

        // Returns field offset
        U32 getFieldOffset(U32 fieldIndex) const;

        // Returns field offset
        U32 getFieldOffset(Semantic const& semantic) const;

        Semantic getFieldSemantic(U32 fieldIndex) const;

        U32 numFields() const;

        U32 getSize() const;

        void reset();
    };

    class LinearStructValue {
    protected:
        LinearStruct* m_pStructure;
        U8* m_pAddr;

    public:
        LinearStructValue(LinearStruct* pStructure, U8* pAddr)
            : m_pStructure(pStructure)
            , m_pAddr(pAddr)
        {
        }

        inline U8* getData(U32 fieldIndex) const
        {
            return m_pAddr + m_pStructure->getFieldOffset(fieldIndex);
        }
    };

    // A stream used for inter component communication
    class FifoStream
    {
    public:
        typedef LinearStructValue Element;

    protected:
        LinearStruct m_structure;

        U8* m_storage;
        U32 m_capacity;

        U32 m_begin;
        U32 m_end;

        friend class StreamBuffer;

    public:
        FifoStream();

        ~FifoStream();

        U32 addChannel(Semantic const& semantic, Type const& type);

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
        typedef LinearStructValue Element;

    protected:
        LinearStruct m_structure;
        U8* m_storage;
        U32 m_length;

    public:
        StreamBuffer() {}

        StreamBuffer(FifoStream const& fifoStream);

        U32 getLength();

        Element getElement(U32 index);

        LinearStruct getElementStruct() const;
    };

} // namespace Device

#endif // _BUFFER_H_
