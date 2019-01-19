#include "pipeline.h"

namespace Device {
    Pipeline::Pipeline()
        : m_inputAssembler{}
        , m_primitiveAssembler{}
        , m_vsProgram{}
        , m_psProgram{}
        , m_rasterizer{}
        , m_outputMerger{}
    {
        // set default target size
        setTargetSize(1024, 768);
    }

    void Pipeline::setVertexBufferChannel(Semantic const& semantic, U8* base, U32 offset, U32 stride)
    {
        m_inputAssembler.setVertexBufferChannel(semantic, base, offset, stride);
    }

    void Pipeline::setVertexBufferLength(U32 length)
    {
        m_inputAssembler.setVertexBufferLength(length);
    }

    void Pipeline::setIndexBuffer(U8* base, U32 offset, U32 stride, U32 length)
    {
        m_inputAssembler.setIndexBuffer(base, offset, stride, length);
    }

    void Pipeline::setVSProgram(Shader& shader)
    {
        m_vsProgram.attach(&shader);
    }

    void Pipeline::setPSProgram(Shader& shader)
    {
        m_psProgram.attach(&shader);
    }

    void Pipeline::setTargetSize(U32 width, U32 height)
    {
        m_rasterizer.resize(width, height);
        m_outputMerger.resize(width, height);
    }

    void Pipeline::present() const
    {
        m_outputMerger.presentToBmp();
    }

    void Pipeline::setupComponents()
    {
        U32 const FIFO_SIZE = 1024 * 1024;

        // setup input buffers.
        // [inputAssember] -> vsInStream -> [vsProgram] -> vsOutStream
        // [inputAssember] -> paInStream -> [primitiveAssembler] -> paOutStream
        m_inputAssembler.setupVertexStream(m_vsInStream);
        m_inputAssembler.setupIndexStream(m_paInStream);

        // setup VS out stream.
        initStream(m_vsOutStream, m_vsProgram, Comp::Output);
        m_vsOutStream.setCapacity(m_vsInStream.getNumElements());

        // setup PA out stream.
        initStream(m_paOutStream, m_primitiveAssembler, Comp::Output);
        m_paOutStream.setCapacity(FIFO_SIZE);

        // setup PS in/out stream.
        initStream(m_psInStream, m_psProgram, Comp::Input);
        m_psInStream.setCapacity(FIFO_SIZE);

        // TODO: should adjust to output merger?
        initStream(m_psOutStream, m_psProgram, Comp::Output);
        m_psOutStream.setCapacity(FIFO_SIZE);

        // setup last dummy stream
        m_dummyStream.setCapacity(1);
    }

    // this function draws everything in the vertex and index buffer.
    void Pipeline::drawIndexed()
    {
        setupComponents();

        // run vertex shader
        runComp(m_vsProgram, m_vsInStream, m_vsOutStream);

        // Assume all vertices are processed.
        assert(m_vsInStream.isEmpty());

        // set the vertex output into rasterizer as a buffer, mark all as processed.
        m_rasterizer.bindVSOutput(m_vsOutStream);

        // setup the rasterizer output ports, keep the same as psProgram.
        m_rasterizer.adjustOutputPorts(m_psProgram);

        while (
            // drain out all component pendings
            m_primitiveAssembler.hasPendingOutput() ||
            m_rasterizer.hasPendingOutput() ||
            m_psProgram.hasPendingOutput() ||
            m_outputMerger.hasPendingOutput() ||
            // drain out all streams
            !m_paInStream.isEmpty() ||
            !m_paOutStream.isEmpty() ||
            !m_psInStream.isEmpty() ||
            !m_psOutStream.isEmpty()
            )
        {
            // run primitive assembler, fill into a 'primitive list' stream.
            runComp(m_primitiveAssembler, m_paInStream, m_paOutStream);

            // run rasterizer, fill into pixel in stream.
            runComp(m_rasterizer, m_paOutStream, m_psInStream);

            // run per-pixel shader, fill into pixel out stream
            runComp(m_psProgram, m_psInStream, m_psOutStream);

            // fill the framebuffer
            runComp(m_outputMerger, m_psOutStream, m_dummyStream);

            // debug
            //static U32 counter = 0;
            //std::cout << counter++ << std::endl;
        }
    }

    void Pipeline::drawIndexed(U32 ibStart, U32 count)
    {
        (void)ibStart;
        (void)count;
    }

} // namespace Device
