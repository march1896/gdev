#include <algorithm>

#include "input_assembler.h"

namespace Device {

    U32 InputAssembler::VertexStream::numChannels() const
    {
        return m_vtxBufEntries.size();
    }

    U32 InputAssembler::VertexStream::getChannelIndex(Semantic const& semantic) const
    {
        BufferEntryList::const_iterator itr = std::find_if(m_vtxBufEntries.begin(), m_vtxBufEntries.end(),
            [&](BufferChannelEntry const& entry) -> bool {
                return entry.semantic == semantic;
            }
        );

        return itr - m_vtxBufEntries.begin();
    }

    U32 InputAssembler::VertexStream::getNumElements() const
    {
        return m_vtxBufLength - m_vtxBufProcessed;
    }

    bool InputAssembler::VertexStream::isEmpty() const
    {
        return m_vtxBufProcessed == m_vtxBufLength;
    }

    bool InputAssembler::VertexStream::isFull() const
    {
        return m_vtxBufProcessed == 0;
    }

    InputAssembler::VertexStream::Element InputAssembler::VertexStream::front()
    {
        return InputAssembler::VertexStream::Element(*this, m_vtxBufProcessed);
    }

    void InputAssembler::VertexStream::popData()
    {
        m_vtxBufProcessed ++;
    }

    ///////////////////////////////////////////////////////////////////////////

    U32 InputAssembler::IndexStream::numChannels() const
    {
        return 1;
    }

    U32 InputAssembler::IndexStream::getChannelIndex(Semantic const& semantic) const
    {
        // TODO: should we assert semantic == SV_Index?
        (void)semantic;
        return 0;
    }

    U32 InputAssembler::IndexStream::getNumElements() const
    {
        return m_idxBufLength - m_idxBufProcessed;
    }

    bool InputAssembler::IndexStream::isEmpty() const
    {
        return m_idxBufProcessed == m_idxBufLength;
    }

    bool InputAssembler::IndexStream::isFull() const
    {
        return m_idxBufProcessed == 0;
    }

    InputAssembler::IndexStream::Element InputAssembler::IndexStream::front()
    {
        U8* addr = m_idxBufEntry.base + m_idxBufEntry.stride * m_idxBufProcessed;
        return InputAssembler::IndexStream::Element(addr);
    }

    void InputAssembler::IndexStream::popData()
    {
        m_idxBufProcessed ++;
    }

    ///////////////////////////////////////////////////////////////////////////

    InputAssembler::InputAssembler()
    {
    }

    void InputAssembler::setVertexBufferChannel(Semantic const& semantic, U8* base, U32 offset, U32 stride)
    {
        BufferEntryList::iterator itr = std::find_if(m_vtxBufEntries.begin(), m_vtxBufEntries.end(),
            [&](BufferChannelEntry const& entry) -> bool {
                return entry.semantic == semantic;
            }
        );

        if (itr == m_vtxBufEntries.end())
        {
            m_vtxBufEntries.push_back(BufferChannelEntry{semantic, base + offset, stride});
        }
        else
        {
            BufferChannelEntry& entry = *itr;
            entry.base = base + offset;
            entry.stride = stride;
        }
    }

    void InputAssembler::setVertexBufferLength(U32 length)
    {
        m_vtxBufLength = length;
    }

    bool InputAssembler::getVertexBufferChannel(Semantic const& semantic, U8** pBase, U32* pStride)
    {
        BufferEntryList::iterator itr = std::find_if(m_vtxBufEntries.begin(), m_vtxBufEntries.end(),
            [&](BufferChannelEntry const& entry) -> bool {
                return entry.semantic == semantic;
            }
        );

        if (itr == m_vtxBufEntries.end())
        {
            return false;
        }

        *pBase = itr->base;
        *pStride = itr->stride;

        return true;
    }

    void InputAssembler::setIndexBuffer(U8* base, U32 offset, U32 stride, U32 length)
    {
        m_idxBufEntry.base = base + offset;
        m_idxBufEntry.stride = stride;
        m_idxBufLength = length;
    }

    void InputAssembler::setupVertexStream(VertexStream& vertexStream)
    {
        vertexStream.m_vtxBufEntries = this->m_vtxBufEntries;
        vertexStream.m_vtxBufLength = this->m_vtxBufLength;
        vertexStream.m_vtxBufProcessed = 0;
    }

    void InputAssembler::setupIndexStream(IndexStream& indexStream)
    {
        indexStream.m_idxBufEntry = this->m_idxBufEntry;
        indexStream.m_idxBufLength = this->m_idxBufLength;
        indexStream.m_idxBufProcessed = 0;
    }
} // namespace Device
