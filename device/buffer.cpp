#include <cassert>

#include "buffer.h"

namespace Device { 
    LinearStruct::LinearStruct()
        : m_fieldOffsets{}
        , m_fieldSemantics{}
        , m_fieldTypes{}
        , m_size{0}
    {
    }

    void LinearStruct::updateOffsets()
    {
        m_size = 0;

        m_fieldOffsets.clear();
        for (U32 fieldIndex = 0; fieldIndex < m_fieldSemantics.size(); ++fieldIndex)
        {
            m_fieldOffsets.push_back(m_size);

            m_size += SizeOf(m_fieldTypes[fieldIndex]);
        }
    }

    // Returns the field index.
    U32 LinearStruct::addField(Semantic const& semantic, Type const& type)
    {
        for (U32 fieldIndex = 0; fieldIndex < m_fieldSemantics.size(); ++fieldIndex)
        {
            Semantic const& fieldSemantic = m_fieldSemantics[fieldIndex];

            if (semantic == fieldSemantic)
            {
                // use semantic as binding idertifier, do not permit override.
                assert(0);
            }
        }

        m_fieldSemantics.push_back(semantic);
        m_fieldTypes.push_back(type);

        // TODO: optimize, we only need to update the last one in this condition.
        updateOffsets();

        return m_fieldTypes.size() - 1;
    }

    U32 LinearStruct::getFieldOffset(U32 fieldIndex) const
    {
        assert(fieldIndex < m_fieldOffsets.size());
        return m_fieldOffsets[fieldIndex];
    }

    U32 LinearStruct::getFieldOffset(Semantic const& semantic) const
    {
        U32 fieldIndex = getFieldIndex(semantic);
        return getFieldOffset(fieldIndex);
    }

    Semantic LinearStruct::getFieldSemantic(U32 fieldIndex) const
    {
        return m_fieldSemantics[fieldIndex];
    }

    U32 LinearStruct::numFields() const
    {
        return m_fieldSemantics.size();
    }

    U32 LinearStruct::getSize() const
    {
        return m_size;
    }

    void LinearStruct::reset()
    {
        m_fieldOffsets.clear();
        m_fieldSemantics.clear();
        m_fieldTypes.clear();
        m_size = 0;
    }

    // Returns the field index.
    U32 LinearStruct::getFieldIndex(Semantic const& semantic) const
    {
        auto const& itr = std::find(m_fieldSemantics.begin(), m_fieldSemantics.end(), semantic);
        return itr - m_fieldSemantics.begin();
    }


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

    U32 FifoStream::addChannel(Semantic const& semantic, Type const& type)
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

        Element element{&m_structure, m_storage + m_end * m_structure.getSize()};
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

        return Element{&m_structure, m_storage + m_begin * m_structure.getSize()};
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
        return Element{&m_structure, m_storage + index * m_structure.getSize()};
    }


    LinearStruct StreamBuffer::getElementStruct() const
    {
        return m_structure;
    }

} // namespace Device
