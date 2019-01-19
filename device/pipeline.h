#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "semantic.h"
#include "buffer.h"
#include "shader.h"
#include "rasterizer.h"
#include "input_assembler.h"
#include "primitive_assembler.h"
#include "shader_processor.h"
#include "output_merger.h"

namespace Device {

    // Initialize stream channels according to component io port.
    template <typename IOStream, typename Component>
    void initStream(IOStream& stream, Component const& comp, Comp::IOType io)
    {
        stream.resetChannels();

        U32 numPorts = comp.getNumPorts(io);
        for (U32 portIndex = 0; portIndex < numPorts; ++ portIndex)
        {
            Semantic const& semantic = comp.getSemantic(io, portIndex);
            Type const& type = comp.getType(io, portIndex);
            stream.addChannel(semantic, type);
        }
    }

    class Pipeline
    {
    protected:
        // Components
        InputAssembler m_inputAssembler;
        PrimitiveAssembler m_primitiveAssembler;
        ShaderProcessor m_vsProgram;
        ShaderProcessor m_psProgram;
        Rasterizer m_rasterizer;
        OutputMerger m_outputMerger;

        // intermediate buffers
        InputAssembler::VertexStream m_vsInStream;
        InputAssembler::IndexStream m_paInStream;

        FifoStream m_vsOutStream;
        FifoStream m_paOutStream;
        FifoStream m_psInStream;
        FifoStream m_psOutStream;
        FifoStream m_dummyStream;

    public:
        Pipeline();

        void setVertexBufferChannel(Semantic const& semantic, U8* base, U32 offset, U32 stride);

        void setVertexBufferLength(U32 length);

        void setIndexBuffer(U8* base, U32 offset, U32 stride, U32 length);

        void setVSProgram(Shader& shader);

        void setPSProgram(Shader& shader);

        void setTargetSize(U32 width, U32 height);

        void present() const;

        void setupComponents();

        // this function draws everything in the vertex and index buffer.
        void drawIndexed();

        void drawIndexed(U32 ibStart, U32 count);
    };
}

#endif // _PIPELINE_H_
