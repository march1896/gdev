#ifndef _INPUT_ASSEMBLER_H_
#define _INPUT_ASSEMBLER_H_

#include <vector>
#include <map>

#include "vmath.h"
#include "semantic.h"
#include "component.h"

namespace Device {
    class InputAssembler
    {
    protected:
        struct BufferChannelEntry
        {
            Semantic semantic;

            U8* base;
            U32 stride;
        };

        typedef std::vector<BufferChannelEntry> BufferEntryList;

    public:
        class VertexStream
        {
        public:
            class Element
            {
            protected:
                VertexStream const& m_vtxStream;
                U32 m_index;

            public:
                Element(VertexStream const& vtxStream, U32 index)
                    : m_vtxStream(vtxStream)
                    , m_index(index)
                {
                }

                inline U8* getData(U32 channel) const
                {
                    return m_vtxStream.getChannelData(m_index, channel);
                }
            };

            friend class InputAssembler;
        protected:
            BufferEntryList m_vtxBufEntries;
            U32 m_vtxBufLength;
            U32 m_vtxBufProcessed; // <$ are processed

            inline U8* getChannelData(U32 index, U32 channel) const
            {
                BufferChannelEntry const& entry = m_vtxBufEntries[channel];
                return entry.base + entry.stride * index;
            }

        public:
            U32 numChannels() const;

            U32 getChannelIndex(Semantic const& semantic) const;

            U32 getNumElements() const;

            bool isEmpty() const;

            bool isFull() const;

            Element front();

            void popData();
        };

        class IndexStream
        {
        public:
            struct Element
            {
            protected:
                U8* m_addr;
            public:
                Element(U8* addr) : m_addr(addr) {}
                inline U8* getData(U32 channel) const
                {
                    (void)channel;
                    return m_addr;
                }
            };

        protected:
            BufferChannelEntry m_idxBufEntry;
            U32 m_idxBufLength;
            U32 m_idxBufProcessed;

            friend class InputAssembler;
        public:
            U32 numChannels() const;

            U32 getChannelIndex(Semantic const& semantic) const;

            U32 getNumElements() const;

            bool isEmpty() const;

            bool isFull() const;

            Element front();

            void popData();
        };

    protected:
        BufferEntryList m_vtxBufEntries;
        U32 m_vtxBufLength;

        BufferChannelEntry m_idxBufEntry;
        U32 m_idxBufLength;

    public:
        InputAssembler();

        void setVertexBufferChannel(Semantic const& semantic, U8* base, U32 offset, U32 stride);

        void setVertexBufferLength(U32 length);

        bool getVertexBufferChannel(Semantic const& semantic, U8** pBase, U32* pStride);

        void setIndexBuffer(U8* base, U32 offset, U32 stride, U32 length);

        void setupVertexStream(VertexStream& vertexStream);

        void setupIndexStream(IndexStream& indexStream);
    };

} // namespace Device

#endif // _INPUT_ASSEMBLER_H_

