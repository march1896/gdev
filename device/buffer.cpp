#include <cassert>

#include "buffer.h"

namespace Device {
    FifoStream::FifoStream()
        : m_structure{}
        , m_storage{nullptr}
        , m_capacity{0u}
        , m_begin{0u}
        , m_end{0u}
    {
    }

    FifoStream::~FifoStream()
    {
        if (m_storage)
        {
            free(m_storage);
            m_storage = nullptr;
        }
    }

    U32 FifoStream::addChannel(Semantic const& semantic, BuiltinType const& type)
    {
        return m_structure.addField(semantic, type);
    }

    U32 FifoStream::getChannelIndex(Semantic const& semantic) const
    {
        return m_structure.getFieldIndex(semantic);
    }

    U32 FifoStream::numChannels() const
    {
        return m_structure.numFields();
    }

    void FifoStream::resetChannels()
    {
        m_structure.reset();
    }

    void FifoStream::setCapacity(U32 maxNumElements)
    {
        m_capacity = maxNumElements + 1;

        if (m_storage)
        {
            free(m_storage);
        }
        m_storage = (U8*)malloc(m_capacity * m_structure.getSize());

        m_begin = m_end = 0;
    }

    U32 FifoStream::getNumElements() const
    {
        return (m_end - m_begin + m_capacity) % m_capacity;
    }

    bool FifoStream::isEmpty() const
    {
        return m_begin == m_end;
    }

    bool FifoStream::isFull() const
    {
        return (m_end + 1) % m_capacity == m_begin;
    }

    FifoStream::Element FifoStream::pushData()
    {
        if (isFull())
        {
            // TODO: change interface?
            assert(0);
        }

        Element element{m_structure, m_storage + m_end * m_structure.getSize()};
        m_end ++;
        m_end %= m_capacity;

        return element;
    }

    FifoStream::Element FifoStream::front()
    {
        if (isEmpty())
        {
            assert(0);
        }

        return Element{m_structure, m_storage + m_begin * m_structure.getSize()};
    }

    void FifoStream::popData()
    {
        m_begin ++;
        m_begin %= m_capacity;
    }

    StreamBuffer::StreamBuffer(FifoStream const& fifoStream)
        : m_structure(fifoStream.m_structure)
        , m_storage(fifoStream.m_storage)
        , m_length(fifoStream.m_capacity)
    {
    }

    U32 StreamBuffer::getLength()
    {
        return m_length;
    }

    StreamBuffer::Element StreamBuffer::getElement(U32 index)
    {
        return Element{m_structure, m_storage + index * m_structure.getSize()};
    }


    StructType StreamBuffer::getElementStruct() const
    {
        return m_structure;
    }

} // namespace Device
