#include <algorithm>

#include "rasterizer.h"

namespace Device {

    Rasterizer::Rasterizer()
        : m_width(1)
        , m_height(1)
    {
        // raster input is connected to primitive assember output.
        addIOPort(Input, std::string("vtx_index"), Type::UINT, Semantic::SV_VertexIndex);
        m_inVtxIdx = getValuePtr(Input, "vtx_index");

        m_triIndex = 0;
    }

    void Rasterizer::resize(U32 width, U32 height)
    {
        m_width = width;
        m_height = height;
    }

    void Rasterizer::setWidth(U32 width)
    {
        m_width = width;
    }

    void Rasterizer::setHeight(U32 height)
    {
        m_height = height;
    }

    U32 Rasterizer::getWidth() const
    {
        return m_width;
    }

    U32 Rasterizer::getHeight() const
    {
        return m_height;
    }

    std::vector<Rasterizer::Triangle> Rasterizer::rasterizeTriangle(Vec4f const& va, Vec4f const& vb, Vec4f const& vc)
    {
        std::vector<Triangle> output;

        // setup triangle
        Triangle2D const triangle = setupTriangle(*(Vec2f*)&va, *(Vec2f*)&vb, *(Vec2f*)&vc);

        // setup NDC(normalized device coordinates) bounding box.
        AABB<float> ndcBox;
        ndcBox.xmin = clamp(std::min({va.x, vb.x, vc.x}), -1.0f, 1.0f);
        ndcBox.xmax = clamp(std::max({va.x, vb.x, vc.x}), -1.0f, 1.0f);
        ndcBox.ymin = clamp(std::min({va.y, vb.y, vc.y}), -1.0f, 1.0f);
        ndcBox.ymax = clamp(std::max({va.y, vb.y, vc.y}), -1.0f, 1.0f);

        U32 const width = m_width;
        U32 const height = m_height;

        // setup screen space bonding box.
        int xmin = std::floor(ndcBox.xmin * width); // left
        int xmax = std::ceil(ndcBox.xmax * width); // right
        int ymin = std::floor(ndcBox.ymin * height); // bottom
        int ymax = std::ceil(ndcBox.ymax * height); // top

        // Note: the nearest odd number
        for (int x = xmin / 2 * 2 + 1; x < xmax; x += 2)
        {
            for (int y = ymin / 2 * 2 + 1; y < ymax; y += 2)
            {
                Vec2f pixel{float(x)/width, float(y)/height};
                // TODO: near/far clipping?
                if (insideTriangle(triangle, pixel))
                {
                    // Prepare(interpolate) pixel input information and pass to pixel shader for rendering.
                    output.push_back(Triangle{});
                    Triangle& rasterInfo = output.back();

                    rasterInfo.x = x;
                    rasterInfo.y = y;
                    rasterInfo.coff = calcBaryCentricCoordinates(triangle, pixel);

                    // debug
                    //{
                    //    BaryCentricCoff const& coff = rasterInfo.coff;
                    //    Vec2f pos = interpolate(va, coff.u, vb, coff.v, vc, coff.w);
                    //    std::cout << U32(pos.x * width) << " " << U32(pos.y * m_height) << " :" << std::endl;
                    //}
                }
            }
        }

        return output;
    }

    void Rasterizer::rasterizeLine(Vec2f const& va, Vec2f const& b)
    {
        // TODO:
        (void)va;
        (void)b;
    }

    void Rasterizer::bindVSOutput(FifoStream& fifoStream)
    {
        m_vsOutBuffer = StreamBuffer{fifoStream};

        LinearStruct const& structure = m_vsOutBuffer.getElementStruct();
        m_vsOutPositionChannel = structure.getFieldIndex(Semantic::SV_Position);

        if (m_vsOutPositionChannel == structure.numFields())
        {
            // Vertex shader does not provide SV_Position as output.
            assert(0);
        }

        // do perspective divide here.
        for (U32 bufIdx = 0; bufIdx < m_vsOutBuffer.getLength(); ++ bufIdx)
        {
            Vec4f& vPos = *(Vec4f*)m_vsOutBuffer.getElement(bufIdx).getData(m_vsOutPositionChannel);

            vPos.x /= vPos.w;
            vPos.y /= vPos.w;
            vPos.z /= vPos.w;
        }
    }

    void Rasterizer::adjustOutputPorts(Comp const& nextComp)
    {
        // clean up old state
        m_types[Comp::Output].clear();
        m_symbols[Comp::Output].clear();
        m_values[Comp::Output].clear();
        m_semantics[Comp::Output].clear();

        // setup new port stats.
        U32 numPorts = nextComp.getNumPorts(Comp::Input);
        for (U32 portIdx = 0; portIdx < numPorts; ++portIdx)
        {
            Semantic const& semantic = nextComp.getSemantic(Comp::Input, portIdx);
            Type const& type = nextComp.getType(Comp::Input, portIdx);
            std::string const& name = nextComp.getName(Comp::Input, portIdx);

            addIOPort(Comp::Output, name, type, semantic);
        }
    }

    bool Rasterizer::isOneInOneOut() const
    {
        return false;
    }

    void Rasterizer::runOne()
    {
        assert(0);
    }

    void Rasterizer::comsumeOneInput()
    {
        m_triVtxIndices[m_triIndex++] = m_inVtxIdx->readAs<U32>();

        if (m_triIndex == 3)
        {
            assert(!hasPendingOutput());

            Vec4f* va = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[0]).getData(m_vsOutPositionChannel);
            Vec4f* vb = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[1]).getData(m_vsOutPositionChannel);
            Vec4f* vc = (Vec4f*)m_vsOutBuffer.getElement(m_triVtxIndices[2]).getData(m_vsOutPositionChannel);

            m_triPending = rasterizeTriangle(*va, *vb, *vc);
            m_triProcessed = 0;

            m_triIndex = 0;
        }
    }

    bool Rasterizer::hasPendingOutput() const
    {
        return m_triProcessed < m_triPending.size();
    }

    void Rasterizer::produceOneOutput()
    {
        assert(m_triProcessed < m_triPending.size());
        Rasterizer::Triangle const& rasterInfo = m_triPending[m_triProcessed++];
        BaryCentricCoff const& coff = rasterInfo.coff;

        // TODO: move this after ps and vsout binding.
        LinearStruct const& structure = m_vsOutBuffer.getElementStruct();

        // Note: assume rasterizer out ports are bound.
        // Note: rasterizer out ports are dynamically constructed, which matchs psIn
        U32 numOutPorts = getNumPorts(Output);
        StreamBuffer::Element element_a = m_vsOutBuffer.getElement(m_triVtxIndices[0]);
        StreamBuffer::Element element_b = m_vsOutBuffer.getElement(m_triVtxIndices[1]);
        StreamBuffer::Element element_c = m_vsOutBuffer.getElement(m_triVtxIndices[2]);

        for (U32 outPortIdx = 0; outPortIdx < numOutPorts; ++outPortIdx)
        {
            Type const& type = getType(Comp::Output, outPortIdx);

            // TODO: cache below values
            Value va{type}, vb{type}, vc{type};
            U32 channel = structure.getFieldIndex(getSemantic(Comp::Output, outPortIdx));

            va.bind(element_a.getData(channel));
            vb.bind(element_b.getData(channel));
            vc.bind(element_c.getData(channel));

            Value* out = getValuePtr(Comp::Output, outPortIdx);
            out->interpolate(&va, coff.u, &vb, coff.v, &vc, coff.w);

            // debug
            //{
            //    if (outPortIdx == 0)
            //    {
            //        // output position
            //        // std::cout << *out << std::endl; // " " <<  va << " " << vb << " " << vc << std::endl;
            //        Vec3f pos = out->readAs<Vec3f>();
            //        std::cout << U32(pos.x * width) << " " << U32(pos.y * height) << std::endl;
            //    }
            //}
        }
    }
}
